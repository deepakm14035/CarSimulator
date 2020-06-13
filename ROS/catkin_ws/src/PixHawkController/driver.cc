/*
 *  Copyright (C) 2015 	Indraprastha Institute of Information Technology, Sanjit Kaul
 * 
 *  License: Modified BSD Software License Agreement
 * 
 *  $Id$
 */

/** \file
 *
 *  ROS driver implementation for the Continental SRR 208-2
 *  Socket reading from can-utils 
 */

#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <libgen.h>
#include <time.h>
#include <errno.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <net/if.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include "terminal.h"
#include "lib.h"

#include "radar_driver/ClusterList.h"
#include "radar_driver/ClusterStatus.h"
#include "radar_driver/RadarCfgStatus.h"

#include <ros/ros.h>
#include <tf/transform_listener.h>
//#include <velodyne_msgs/VelodyneScan.h>

#include "driver.h"

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

void sigterm(int signo)
{
	running = 0;
}

using namespace radar_driver;

void RadarDriver::decodeClusterStatus(struct canfd_frame *cf) {

ClusterStatus status;

status.numClusters =  (unsigned int) (cf->data[0] & 0x3F);
status.clusterStatusRollCount =  (unsigned int) (cf->data[1] & 0x03);

clusterStatus.publish(status);

std::cout << "NoOfClusters " << (unsigned int) (cf->data[0] & 0x3F) << " ClusterStatusRollCount " << (unsigned int) (cf->data[1] & 0x03) << std::endl;

}
void RadarDriver::decodeClusterList(struct canfd_frame *cf) {

radar_driver::ClusterList list;

list.clusterIndex = (unsigned int) (cf->data[0]);
list.rcs = ((float) (cf->data[1])) * 0.5 - 50; 
list.range = ((float) (cf->data[2])) * 0.2;
list.azimuth = ((float) (cf->data[3] & 0x7F)) * 2 - 90; 
list.clusterRollCount = (unsigned int) ((cf->data[4] & 0xC0) >> 6); 
list.vel = ((float) (((0x0000 | (cf->data[4] & 0x07)) << 8) | (0x0000 | (cf->data[5]))) * 0.05) - (float) 35;

clusterList.publish(list);

std::cout << "ClusterIndex " << list.clusterIndex << " ClusterRCS (dBm^2) " << list.rcs << " ClusterRange (m) " << list.range << " ClusterAzimuth (deg) " << list.azimuth << " ClusterRollCount " << list.clusterRollCount << " ClusterVelocity " << list.vel << std::endl;
}
void RadarDriver::decodeRadarStatus(struct canfd_frame *cf) {

radar_driver::RadarCfgStatus status;

status.actl =  (unsigned int) (cf->data[0] & 0x3F);
status.cfgStatus =  (unsigned int) ((cf->data[1] & 0xF0) >> 4);
status.statusRollCount =  (unsigned int) (cf->data[1] & 0x03);

radarCfgStatus.publish(status);

std::cout << "ACTL_Mode" << (unsigned int) (cf->data[0] & 0x3F) << " Radar Cfg Status " << (unsigned int) ((cf->data[1] & 0xF0) >> 4) << " Radar Status Roll Count " << (unsigned int) (cf->data[1] & 0x03) << std::endl;
// Rest is empty
}

void RadarDriver::processCANMessage(struct canfd_frame *cf) {

//std::cout << "The CAN ID of the message is " << cf->can_id << std::endl;

if(cf->can_id == (unsigned int) 0x60A)
{
	//std::cout << "Received a CAN configuration message" << std::endl;
	decodeRadarStatus(cf);
}
if(cf->can_id == (unsigned int) 0x70B)
{
	//std::cout << "Received a CAN cluster status message" << std::endl;
	decodeClusterStatus(cf);
}
if(cf->can_id == (unsigned int) 0x70C)
{
	//std::cout << "Received a CAN cluster info message" << std::endl;
	decodeClusterList(cf);
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


namespace radar_driver
{

	RadarDriver::RadarDriver(ros::NodeHandle node,
			ros::NodeHandle private_nh)
	{
		// use private node handle to get parameters

		//private_nh.param("name", name, std::string("can0"));
		name = std::string("can0");
		std::string tf_prefix = tf::getPrefixParam(private_nh);
		ROS_DEBUG_STREAM("tf_prefix: " << tf_prefix);
		//config_.frame_id = tf::resolve(tf_prefix, config_.frame_id);

		// raw data output topic
		//output_ = node.advertise<velodyne_msgs::VelodyneScan>("radar_packets", 10);
  		
		radarCfgStatus = node.advertise<radar_driver::RadarCfgStatus>("RadarCfg", 10);
		clusterStatus = node.advertise<radar_driver::ClusterStatus>("ClusterStatus", 10);
		clusterList = node.advertise<radar_driver::ClusterList>("ClusterList", 10);
	}

	/** poll the device
	 *
	 *  @returns true unless end of file reached
	 */
	bool RadarDriver::poll(void)
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


		s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
		if (s < 0) {
			perror("socket");
			return 1;
		}

		addr.can_family = AF_CAN;

		memset(&ifr.ifr_name, 0, sizeof(ifr.ifr_name));
		strncpy(ifr.ifr_name, name.c_str(), name.length());

#ifdef DEBUG
		printf("using interface name '%s'.\n", ifr.ifr_name);
#endif

		if (strcmp(ANYDEV, ifr.ifr_name)) {
			if (ioctl(s, SIOCGIFINDEX, &ifr) < 0) {
				perror("SIOCGIFINDEX");
				exit(1);
			}
			addr.can_ifindex = ifr.ifr_ifindex;
		} else
			addr.can_ifindex = 0; /* any can interface */


		/* try to switch the socket into CAN FD mode */
		setsockopt(s, SOL_CAN_RAW, CAN_RAW_FD_FRAMES, &canfd_on, sizeof(canfd_on));
#ifdef IGNORE_SKK
		if (rcvbuf_size) {

			int curr_rcvbuf_size;
			socklen_t curr_rcvbuf_size_len = sizeof(curr_rcvbuf_size);

			/* try SO_RCVBUFFORCE first, if we run with CAP_NET_ADMIN */
			if (setsockopt(s, SOL_SOCKET, SO_RCVBUFFORCE,
						&rcvbuf_size, sizeof(rcvbuf_size)) < 0) {
#ifdef DEBUG
				printf("SO_RCVBUFFORCE failed so try SO_RCVBUF ...\n");
#endif
				if (setsockopt(s, SOL_SOCKET, SO_RCVBUF,
							&rcvbuf_size, sizeof(rcvbuf_size)) < 0) {
					perror("setsockopt SO_RCVBUF");
					return 1;
				}

				if (getsockopt(s, SOL_SOCKET, SO_RCVBUF,
							&curr_rcvbuf_size, &curr_rcvbuf_size_len) < 0) {
					perror("getsockopt SO_RCVBUF");
					return 1;
				}

				/* Only print a warning the first time we detect the adjustment */
				/* n.b.: The wanted size is doubled in Linux in net/sore/sock.c */
				if (!i && curr_rcvbuf_size < rcvbuf_size*2)
					fprintf(stderr, "The socket receive buffer size was "
							"adjusted due to /proc/sys/net/core/rmem_max.\n");
			}
		}
#endif

		if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
			perror("bind");
			return 1;
		}

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
				running = 0;
				continue;
			}

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
						return 1;
					}


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

} // namespace radar_driver
