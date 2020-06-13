/*
 *  Copyright (C) 2015 Indraprastha Institute of Information Technology, Sanjit Kaul
 * 
 *  License: Modified BSD Software License Agreement
 *
 *  $Id$
 */

/** \file
 *
 *  CAN Node for talking to RADAR and e2o
 */

#include <ros/ros.h>
#include "can_driver.h"
#include <signal.h>
#include <iostream>
#include <iomanip>
#include "can_node.cc"

#define MAXIFNAMES 30 /* size of receive name index to omit ioctls */
#define MAXCOL 6      /* number of different colors for colorized output */
#define ANYDEV "any"  /* name of interface to receive from any CAN interface */
#define ANL "\r\n"    /* newline in ASC mode */

#define SILENT_INI 42 /* detect user setting on commandline */
#define SILENT_OFF 0  /* no silent mode */
#define SILENT_ANI 1  /* silent mode with animation */
#define SILENT_ON  2  /* silent mode (completely silent) */

#define MAXANI 4
const char hex_asc_upper[] = "0123456789ABCDEF";

#define hex_asc_upper_lo(x)	hex_asc_upper[((x) & 0x0F)]
#define hex_asc_upper_hi(x)	hex_asc_upper[((x) & 0xF0) >> 4]

static inline void put_hex_byte(char *buf, __u8 byte)
{
	buf[0] = hex_asc_upper_hi(byte);
	buf[1] = hex_asc_upper_lo(byte);
}

static inline void _put_id(char *buf, int end_offset, canid_t id)
{
	/* build 3 (SFF) or 8 (EFF) digit CAN identifier */
	while (end_offset >= 0) {
		buf[end_offset--] = hex_asc_upper[id & 0xF];
		id >>= 4;
	}
}
#define put_sff_id(buf, id) _put_id(buf, 2, id)
#define put_eff_id(buf, id) _put_id(buf, 7, id)
const char anichar[MAXANI] = {'|', '/', '-', '\\'};


static char *cmdlinename;
static __u32 dropcnt;
static __u32 last_dropcnt;
static char devname[MAXIFNAMES][IFNAMSIZ+1];
static int  dindex[MAXIFNAMES];
static int  max_devname_len; /* to prevent frazzled device name output */ 
const int canfd_on = 1;
static volatile int running = 1;

int velocity = 0.0;
int throttle = 0.0;
int brake = 0.0;
int steering = 0.0;
int steering_direc = 0;

void sigterm(int signo)
{
	running = 0;
}

using namespace can_driver;

bool CANObject::poll()
{
		fd_set rdfs;
		struct ifreq ifr;
		int s;
		int bridge = 0;
		useconds_t bridge_delay = 0;

		int rcvbuf_size = 0;
		int currmax = 1;
		struct sockaddr_can addr;
		unsigned char timestamp = 0;
		unsigned char dropmonitor = 0;
		unsigned char extra_msg_info = 0;
		unsigned char silent = SILENT_INI;
		unsigned char silentani = 0;
		unsigned char color = 0;
		unsigned char view = 0;
		unsigned char log = 0;
		unsigned char logfrmt = 0;
		int count = 0;
		int opt, ret;
		int join_filter;
		char *ptr, *nptr;
		char ctrlmsg[CMSG_SPACE(sizeof(struct timeval)) + CMSG_SPACE(sizeof(__u32))];
		struct iovec iov;
		struct msghdr msg;
		struct cmsghdr *cmsg;
		struct can_filter *rfilter;
		can_err_mask_t err_mask;
		struct canfd_frame frame;
		int nbytes, i, maxdlen;
		struct timeval tv, last_tv;
		struct timeval timeout, timeout_config = { 0, 0 }, *timeout_current = NULL;
		FILE *logfile = NULL;
    
    CANObject::initCAN();

    s = CANObject::can_socket;
    addr = CANObject::can_addr;

		/* these settings are static and can be held out of the hot path */
		iov.iov_base = &frame;
		msg.msg_name = &addr;
		msg.msg_iov = &iov;
		msg.msg_iovlen = 1;
		msg.msg_control = &ctrlmsg;

		while (running) {

			FD_ZERO(&rdfs);
			for (i=0; i<currmax; i++)
				FD_SET(s, &rdfs);

			if (timeout_current)
				*timeout_current = timeout_config;


			if ((ret = select(s+1, &rdfs, NULL, NULL, timeout_current)) <= 0) {
				//perror("select");
        std::cout << "Error in select" << std::endl;
				running = 0;
				continue;
			}

        std::cout << "Succ in select" << std::endl;
			for (i=0; i<currmax; i++) {  /* check all CAN RAW sockets */

				if (FD_ISSET(s, &rdfs)) {

					int idx;

					/* these settings may be modified by recvmsg() */
					iov.iov_len = sizeof(frame);
					msg.msg_namelen = sizeof(addr);
					msg.msg_controllen = sizeof(ctrlmsg);  
					msg.msg_flags = 0;

					nbytes = recvmsg(s, &msg, 0);
					if (nbytes < 0) {
						perror("read");
        std::cout << "RCV msg failed in select" << std::endl;
						return 1;
					}

        std::cout << "RCV msg succ in select" << std::endl;

					if ((size_t)nbytes == CAN_MTU)
						maxdlen = CAN_MAX_DLEN;
					else if ((size_t)nbytes == CANFD_MTU)
						maxdlen = CANFD_MAX_DLEN;
					else {
						fprintf(stderr, "read: incomplete CAN frame\n");
						return 1;
					}

					if (count && (--count == 0))
						running = 0;

					if (bridge) {
						if (bridge_delay)
							usleep(bridge_delay);

						nbytes = write(bridge, &frame, nbytes);
						if (nbytes < 0) {
							perror("bridge write");
							return 1;
						} else if ((size_t)nbytes != CAN_MTU && (size_t)nbytes != CANFD_MTU) {
							fprintf(stderr,"bridge write: incomplete CAN frame\n");
							return 1;
						}
					}

					for (cmsg = CMSG_FIRSTHDR(&msg);
							cmsg && (cmsg->cmsg_level == SOL_SOCKET);
							cmsg = CMSG_NXTHDR(&msg,cmsg)) {
						if (cmsg->cmsg_type == SO_TIMESTAMP)
							tv = *(struct timeval *)CMSG_DATA(cmsg);
						else if (cmsg->cmsg_type == SO_RXQ_OVFL)
							dropcnt = *(__u32 *)CMSG_DATA(cmsg);
					}

					/* check for (unlikely) dropped frames on this specific socket */
					if (dropcnt != last_dropcnt) {

						__u32 frames = dropcnt - last_dropcnt;

						if (silent != SILENT_ON)
							printf("DROPCOUNT: dropped %d CAN frame%s on '%s' socket (total drops %d)\n",
									frames, (frames > 1)?"s":"", cmdlinename, dropcnt);

						if (log)
							fprintf(logfile, "DROPCOUNT: dropped %d CAN frame%s on '%s' socket (total drops %d)\n",
									frames, (frames > 1)?"s":"", cmdlinename, dropcnt);

						last_dropcnt = dropcnt;
					}

					idx = idx2dindex(addr.can_ifindex, s);

					/* once we detected a EFF frame indent SFF frames accordingly */
					if (frame.can_id & CAN_EFF_FLAG)
						view |= CANLIB_VIEW_INDENT_SFF;

					if (log) {
						char buf[CL_CFSZ]; /* max length */

						/* log CAN frame with absolute timestamp & device */
						//SKK sprint_canframe(buf, &frame, 0, maxdlen);
						fprintf(logfile, "(%010ld.%06ld) %*s %s\n",
								tv.tv_sec, tv.tv_usec,
								max_devname_len, devname[idx], buf);
					}

					if (logfrmt) {
						char buf[CL_CFSZ]; /* max length */

						/* print CAN frame in log file style to stdout */
						// SKK sprint_canframe(buf, &frame, 0, maxdlen);
						printf("(%010ld.%06ld) %*s %s\n",
								tv.tv_sec, tv.tv_usec,
								max_devname_len, devname[idx], buf);
						goto out_fflush; /* no other output to stdout */
					}
					char buf[CL_CFSZ]; /* max length */
					// extract_info(&frame, velocity, throttle, brake, steering, steering_direc);
					skk_sprint_canframe(buf, &frame, 0, maxdlen);
					std::cout << buf << std::endl;
					processCANMessage(&frame);
					//Data is buf[4] onward
					if (silent != SILENT_OFF){
						if (silent == SILENT_ANI) {
							printf("%c\b", anichar[silentani%=MAXANI]);
							silentani++;
						}
						goto out_fflush; /* no other output to stdout */
					}

				}

out_fflush:
				fflush(stdout);
			}
		}

		for (i=0; i<currmax; i++)
			close(s);

		if (bridge)
			close(bridge);

		if (log)
			fclose(logfile);
		return true;
	}

void CANObject::processCANMessage(struct canfd_frame *frame)
{
	//Route messages from e2o and RADAR 
	//copy data
	unsigned char hex_data[8];
	for(int i=0;i<8;i++)
	{
		hex_data[i] = frame->data[i];
	} 

	if(frame->can_id == 0x76C)//vehicle speed message
	{
		velocity = (int)hex_data[7];
		std::cout<<"--------------------_velocity : "<<(int)hex_data[7]<<"\n";
	}
	if(frame->can_id == 0x773)//brake message
	{
		hex_data[7] = hex_data[7]>>1;
		brake = (int)hex_data[7];
		std::cout<<"--------------------_brake : "<<(int)hex_data[7]<<"\n";
	}
	if(frame->can_id == 0x771)//throttle message
	{
		hex_data[7] = hex_data[7]>>1;
		throttle = (int)hex_data[7];
		std::cout<<"--------------------_throttle : "<<(int)hex_data[7]<<"\n";
	}
		if(frame->can_id == 0x775)//steering message
		{
			//left 1
			//right 0
			char mask = 0x03;
			char last_two = hex_data[7] & mask;
			if(last_two == 0x01)
			{
				steering_direc = 1;//left
				std::cout<<"-------------------- going right "<<(int)hex_data[7]<<"\n";
			}
			else
			{
				steering_direc = 0;//right
				std::cout<<"-------------------- going left : "<<(int)hex_data[7]<<"\n";
			}
			hex_data[7] = hex_data[7]>>1;
			hex_data[7] = hex_data[7]>>1;
			steering = (int)hex_data[7];
			std::cout<<"--------------------_steering : "<<(int)hex_data[7]<<"\n";
		}

		e2o_message.velocity = velocity;
	    e2o_message.throttle = throttle;
	    e2o_message.brake = brake;
	    e2o_message.steering = steering;
	    e2o_message.steering_direc = steering_direc;
	    e2o_message.header.stamp = ros::Time::now();
	    std::cout<<"I am trying to publish\n";
	    e2o_broadcaster.publish(e2o_message);
	return;
}

void CANObject::initCAN()
{
    if(CANObject::can_socket != -1)
    {
      //CAN already initialized
      return;
    }

		int s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
		if (s < 0) {
			perror("socket");
			return;
		}

    struct sockaddr_can addr;
    struct ifreq ifr;


		addr.can_family = AF_CAN;

		memset(&ifr.ifr_name, 0, sizeof(ifr.ifr_name));
		strncpy(ifr.ifr_name, CANObject::can_name.c_str(), CANObject::can_name.length());

		if (ioctl(s, SIOCGIFINDEX, &ifr) < 0) {
				perror("SIOCGIFINDEX");
        close(s);
        return;
    }
		addr.can_ifindex = ifr.ifr_ifindex;

		/* try to switch the socket into CAN FD mode */
		setsockopt(s, SOL_CAN_RAW, CAN_RAW_FD_FRAMES, &canfd_on, sizeof(canfd_on));

		if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
			perror("bind");
      close(s);
			return;
		}
    // All is good. CAN ready to use.
    CANObject::can_socket = s;
    CANObject::can_addr = addr;

    return;
}

void CANObject::sendMsgOverCAN_FD(struct canfd_frame* frame)
{
    CANObject::initCAN();
    int nbytes = sendto(CANObject::can_socket, frame, sizeof(struct canfd_frame),
                    0, (struct sockaddr*)& CANObject::can_addr, sizeof(CANObject::can_addr));

    if(nbytes < sizeof(CANObject::can_addr))
    {
      std::cout << "Error in CANObject::sendMsgOverCAN_FD" << std::endl;
    }
}

void CANObject::sendMsgOverCAN(struct can_frame* frame)
{
    CANObject::initCAN();
    int nbytes = sendto(CANObject::can_socket, frame, sizeof(struct can_frame),
                    0, (struct sockaddr*)& CANObject::can_addr, sizeof(CANObject::can_addr));

    if(nbytes < sizeof(CANObject::can_addr))
    {
      std::cout << "Error in CANObject::sendMsgOverCAN" << std::endl;
    }
}
void skk_sprint_canframe(char *buf , struct canfd_frame *cf, int sep, int maxdlen) {
	/* documentation see lib.h */

	int i,offset;
	int len = (cf->len > maxdlen) ? maxdlen : cf->len;

	if (cf->can_id & CAN_ERR_FLAG) {
		put_eff_id(buf, cf->can_id & (CAN_ERR_MASK|CAN_ERR_FLAG));
		buf[8] = '#';
		offset = 9;
	} else if (cf->can_id & CAN_EFF_FLAG) { //SKK: Extended frame format
		put_eff_id(buf, cf->can_id & CAN_EFF_MASK);
		buf[8] = '#';
		offset = 9;
	} else {
		put_sff_id(buf, cf->can_id & CAN_SFF_MASK); //SKK: Standard frame format has 12 bits as id. Represented as hex we get three characters
		buf[3] = '#';
		offset = 4;
	}

	/* standard CAN frames may have RTR enabled. There are no ERR frames with RTR */
	if (maxdlen == CAN_MAX_DLEN && cf->can_id & CAN_RTR_FLAG) {
		buf[offset++] = 'R';
		/* print a given CAN 2.0B DLC if it's not zero */
		if (cf->len && cf->len <= CAN_MAX_DLC)
			buf[offset++] = hex_asc_upper[cf->len & 0xF];

		buf[offset] = 0;
		return;
	}

	if (maxdlen == CANFD_MAX_DLEN) {
		/* add CAN FD specific escape char and flags */
		buf[offset++] = '#';
		buf[offset++] = hex_asc_upper[cf->flags & 0xF];
		if (sep && len)
			buf[offset++] = '.';
	}

	for (i = 0; i < len; i++) {
		put_hex_byte(buf + offset, cf->data[i]);
		offset += 2;
		if (sep && (i+1 < len))
			buf[offset++] = '.';
	}

	buf[offset] = 0;
}

int idx2dindex(int ifidx, int socket) {

	int i;
	struct ifreq ifr;

	for (i=0; i < MAXIFNAMES; i++) {
		if (dindex[i] == ifidx)
			return i;
	}

	/* create new interface index cache entry */

	/* remove index cache zombies first */
	for (i=0; i < MAXIFNAMES; i++) {
		if (dindex[i]) {
			ifr.ifr_ifindex = dindex[i];
			if (ioctl(socket, SIOCGIFNAME, &ifr) < 0)
				dindex[i] = 0;
		}
	}

	for (i=0; i < MAXIFNAMES; i++)
		if (!dindex[i]) /* free entry */
			break;

	if (i == MAXIFNAMES) {
		fprintf(stderr, "Interface index cache only supports %d interfaces.\n",
				MAXIFNAMES);
		exit(1);
	}

	dindex[i] = ifidx;

	ifr.ifr_ifindex = ifidx;
	if (ioctl(socket, SIOCGIFNAME, &ifr) < 0)
		perror("SIOCGIFNAME");

	if (max_devname_len < strlen(ifr.ifr_name))
		max_devname_len = strlen(ifr.ifr_name);

	strcpy(devname[i], ifr.ifr_name);

#ifdef DEBUG
	printf("new index %d (%s)\n", i, devname[i]);
#endif

	return i;
}
