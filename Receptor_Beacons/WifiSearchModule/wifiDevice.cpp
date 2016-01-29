#include "wifiDevice.h"
//#include "iwlist-SRC/iwlist.c" 
#include "iwlist-SRC/iwlistMinimal.c"

WifiDevice::WifiDevice(string deviceName) {
    Name = deviceName;
}

//list<TrafficSign>* WifiDevice::ScanForTSMATrafficSigns() {
//    list<WifiNetwork>* tsmaWifiNetworks = WifiDevice::ScanForTSMAWifiNetworks();



    //ScanForMessages();
//}

list<WifiNetwork>* WifiDevice::ScanForTSMAWifiNetworks() {
    list<WifiNetwork>* tsmaWifiNetworks = new list<WifiNetwork>();
    int skfd;			/* generic raw socket desc.	*/

    /* Create a channel to the NET kernel. */
    if((skfd = iw_sockets_open()) < 0)
    {
        perror("socket");
        return tsmaWifiNetworks;
        //return -1;
    }

    char* ifname = (char*)Name.c_str();
    list<WifiNetwork> foundNetworks = ScanForWiFiNetworks(skfd, ifname, NULL, 0);

    for (list<WifiNetwork>::const_iterator wifiIter = foundNetworks.begin(); wifiIter != foundNetworks.end(); ++wifiIter) {
        WifiNetwork foundNetwork = *wifiIter;

        if (!foundNetwork.TSMACodedMessages.empty()) {
            tsmaWifiNetworks->push_back(foundNetwork);
            //std::stringstream outputSS;

            //string decipheredMessage;

            //for (list<string>::const_iterator msgIter = foundNetwork.TSMACodedMessages.begin(); msgIter != foundNetwork.TSMACodedMessages.end(); ++msgIter) {
            //    string codedMessage = *msgIter;
            //    //string partialMessage = Util::HexToString(codedMessage);
            //    decipheredMessage.append(codedMessage);
            //}

            //outputSS << "Rede: '" << foundNetwork.SSID << "' -> " << decipheredMessage;
            //foundMessages.push_back(outputSS.str());
        }
    }

    /* Close the socket. */
    iw_sockets_close(skfd);

    return tsmaWifiNetworks;
}

list<string> WifiDevice::ScanForMessages()
{
    return WifiDevice::ScanForMessages("");
}

list<string> WifiDevice::ScanForMessages(string channel) // list_available_wifi_cells
{
    list<string> foundMessages;
    int skfd;			/* generic raw socket desc.	*/

    /* Create a channel to the NET kernel. */
    if((skfd = iw_sockets_open()) < 0)
    {
        perror("socket");
        return list<string>();
        //return -1;
    }

    char* ifname = (char*)Name.c_str();
    list<WifiNetwork> foundNetworks = ScanForWiFiNetworks(skfd, ifname, NULL, 0);

    for (list<WifiNetwork>::const_iterator wifiIter = foundNetworks.begin(); wifiIter != foundNetworks.end(); ++wifiIter) {
        WifiNetwork foundNetwork = *wifiIter;
        if (!foundNetwork.TSMACodedMessages.empty()) {
            std::stringstream outputSS;

            string decipheredMessage;

            for (list<string>::const_iterator msgIter = foundNetwork.TSMACodedMessages.begin(); msgIter != foundNetwork.TSMACodedMessages.end(); ++msgIter) {
                string codedMessage = *msgIter;
                //string partialMessage = Util::HexToString(codedMessage);
                decipheredMessage.append(codedMessage);
            }

            outputSS << "Rede: '" << foundNetwork.SSID << "' -> " << decipheredMessage;
            foundMessages.push_back(outputSS.str());
        }
    }

    /* Close the socket. */
    iw_sockets_close(skfd);

    return foundMessages;
}

list<WifiNetwork> WifiDevice::ScanForWiFiNetworks(int skfd, char* ifname, char* args[], int count)
{
    struct iwreq		wrq;
    struct iw_scan_req    scanopt;		/* Options for 'set' */
    int			scanflags = 0;		/* Flags for scan */
    unsigned char *	buffer = NULL;		/* Results */
    int			buflen = IW_SCAN_MAX_DATA; /* Min for compat WE<17 */
    struct iw_range	range;
    int			has_range;
    struct timeval	tv;				/* Select timeout */
    int			timeout = 15000000;		/* 15s */
    list<WifiNetwork> foundNetworks;

    /* Avoid "Unused parameter" warning */
    args = args; count = count;

    /* Debugging stuff */
    if((IW_EV_LCP_PK2_LEN != IW_EV_LCP_PK_LEN) || (IW_EV_POINT_PK2_LEN != IW_EV_POINT_PK_LEN))
    {
        fprintf(stderr, "*** Please report to jt@hpl.hp.com your platform details\n");
        fprintf(stderr, "*** and the following line :\n");
        fprintf(stderr, "*** IW_EV_LCP_PK2_LEN = %zu ; IW_EV_POINT_PK2_LEN = %zu\n\n",
                IW_EV_LCP_PK2_LEN, IW_EV_POINT_PK2_LEN);
    }

    /* Get range stuff */
    has_range = (iw_get_range_info(skfd, ifname, &range) >= 0);

    /* Check if the interface could support scanning. */
    if((!has_range) || (range.we_version_compiled < 14))
    {
        fprintf(stderr, "%-8.16s  Interface doesn't support scanning.\n\n",
                ifname);
        return foundNetworks;
    }

    /* Init timeout value -> 250ms between set and first get */
    tv.tv_sec = 0;
    tv.tv_usec = 250000;

    /* Clean up set args */
    memset(&scanopt, 0, sizeof(scanopt));

    /* Parse command line arguments and extract options.
   * Note : when we have enough options, we should use the parser
   * from iwconfig... */
    while(count > 0)
    {
        /* One arg is consumed (the option name) */
        count--;

        /*
       * Check for Active Scan (scan with specific essid)
       */
        if(!strncmp(args[0], "essid", 5))
        {
            if(count < 1)
            {
                fprintf(stderr, "Too few arguments for scanning option [%s]\n",
                        args[0]);
                return foundNetworks;
            }
            args++;
            count--;

            /* Store the ESSID in the scan options */
            scanopt.essid_len = strlen(args[0]);
            memcpy(scanopt.essid, args[0], scanopt.essid_len);
            /* Initialise BSSID as needed */
            if(scanopt.bssid.sa_family == 0)
            {
                scanopt.bssid.sa_family = ARPHRD_ETHER;
                memset(scanopt.bssid.sa_data, 0xff, ETH_ALEN);
            }
            /* Scan only this ESSID */
            scanflags |= IW_SCAN_THIS_ESSID;
        }
        else
            /* Check for last scan result (do not trigger scan) */
            if(!strncmp(args[0], "last", 4))
            {
                /* Hack */
                scanflags |= IW_SCAN_HACK;
            }
            else
            {
                fprintf(stderr, "Invalid scanning option [%s]\n", args[0]);
                return foundNetworks;
            }

        /* Next arg */
        args++;
    }

    /* Check if we have scan options */
    if(scanflags)
    {
        wrq.u.data.pointer = (caddr_t) &scanopt;
        wrq.u.data.length = sizeof(scanopt);
        wrq.u.data.flags = scanflags;
    }
    else
    {
        wrq.u.data.pointer = NULL;
        wrq.u.data.flags = 0;
        wrq.u.data.length = 0;
    }

    /* If only 'last' was specified on command line, don't trigger a scan */
    if(scanflags == IW_SCAN_HACK)
    {
        /* Skip waiting */
        tv.tv_usec = 0;
    }
    else
    {
        /* Initiate Scanning */
        if(iw_set_ext(skfd, ifname, SIOCSIWSCAN, &wrq) < 0)
        {
            if((errno != EPERM) || (scanflags != 0))
            {
                fprintf(stderr, "%-8.16s  Interface doesn't support scanning : %s\n\n",
                        ifname, strerror(errno));
                return foundNetworks;
            }
            /* If we don't have the permission to initiate the scan, we may
       * still have permission to read left-over results.
       * But, don't wait !!! */
#if 0
            /* Not cool, it display for non wireless interfaces... */
            fprintf(stderr, "%-8.16s  (Could not trigger scanning, just reading left-over results)\n", ifname);
#endif
            tv.tv_usec = 0;
        }
    }
    timeout -= tv.tv_usec;

    /* Forever */
    while(1)
    {
        fd_set		rfds;		/* File descriptors for select */
        int		last_fd;	/* Last fd */
        int		ret;

        /* Guess what ? We must re-generate rfds each time */
        FD_ZERO(&rfds);
        last_fd = -1;

        /* In here, add the rtnetlink fd in the list */

        /* Wait until something happens */
        ret = select(last_fd + 1, &rfds, NULL, NULL, &tv);

        /* Check if there was an error */
        if(ret < 0)
        {
            if(errno == EAGAIN || errno == EINTR)
                continue;
            fprintf(stderr, "Unhandled signal - exiting...\n");
            return foundNetworks;
        }

        /* Check if there was a timeout */
        if(ret == 0)
        {
            unsigned char *	newbuf;

realloc:
            /* (Re)allocate the buffer - realloc(NULL, len) == malloc(len) */
            newbuf = static_cast<unsigned char*>(realloc(buffer, buflen));
            if(newbuf == NULL)
            {
                if(buffer)
                    free(buffer);
                fprintf(stderr, "%s: Allocation failed\n", __FUNCTION__);
                return foundNetworks;
            }
            buffer = newbuf;

            /* Try to read the results */
            wrq.u.data.pointer = buffer;
            wrq.u.data.flags = 0;
            wrq.u.data.length = buflen;
            if(iw_get_ext(skfd, ifname, SIOCGIWSCAN, &wrq) < 0)
            {
                /* Check if buffer was too small (WE-17 only) */
                if((errno == E2BIG) && (range.we_version_compiled > 16)
                        && (buflen < 0xFFFF))
                {
                    /* Some driver may return very large scan results, either
                   * because there are many cells, or because they have many
                   * large elements in cells (like IWEVCUSTOM). Most will
                   * only need the regular sized buffer. We now use a dynamic
                   * allocation of the buffer to satisfy everybody. Of course,
                   * as we don't know in advance the size of the array, we try
                   * various increasing sizes. Jean II */

                    /* Check if the driver gave us any hints. */
                    if(wrq.u.data.length > buflen)
                        buflen = wrq.u.data.length;
                    else
                        buflen *= 2;

                    /* wrq.u.data.length is 16 bits so max size is 65535 */
                    if(buflen > 0xFFFF)
                        buflen = 0xFFFF;

                    /* Try again */
                    goto realloc;
                }

                /* Check if results not available yet */
                if(errno == EAGAIN)
                {
                    /* Restart timer for only 100ms*/
                    tv.tv_sec = 0;
                    tv.tv_usec = 100000;
                    timeout -= tv.tv_usec;
                    if(timeout > 0)
                        continue;	/* Try again later */
                }

                /* Bad error */
                free(buffer);
                fprintf(stderr, "%-8.16s  Failed to read scan data : %s\n\n",
                        ifname, strerror(errno));
                return foundNetworks;
            }
            else
                /* We have the results, go to process them */
                break;
        }

        /* In here, check if event and event type
       * if scan event, read results. All errors bad & no reset timeout */
    }

    if(wrq.u.data.length)
    {
        struct iw_event		iwe;
        struct stream_descr	stream;
        struct iwscan_state	state = { 1, 0 };
        int			ret;

#ifdef DEBUG
        /* Debugging code. In theory useless, because it's debugged ;-) */
        int	i;
        printf("Scan result %d [%02X", wrq.u.data.length, buffer[0]);
        for(i = 1; i < wrq.u.data.length; i++)
            printf(":%02X", buffer[i]);
        printf("]\n");
#endif
        //printf("%-8.16s  Scan completed :\n", ifname);
        iw_init_event_stream(&stream, (char *) buffer, wrq.u.data.length);
        WifiNetwork* lastFoundWifiNetwork = NULL;
        do
        {
            WifiNetwork* foundNetwork;
            /* Extract an event and print it */
            ret = iw_extract_event_stream(&stream, &iwe, range.we_version_compiled);
            if(ret > 0)
                foundNetwork = RetrieveWifiNetworkInfo(&stream, &iwe, &state, &range, has_range, lastFoundWifiNetwork);

            if(foundNetwork != NULL) { // Encontrei uma Nova
                if (lastFoundWifiNetwork != NULL) // Já tiver encontrado alguma uma vez
                    foundNetworks.push_back(*lastFoundWifiNetwork);
                lastFoundWifiNetwork = foundNetwork;
            }
        }
        while(ret > 0);
        if (lastFoundWifiNetwork != NULL)
            foundNetworks.push_back(*lastFoundWifiNetwork);
    }
    else
        printf("%-8.16s  No scan results\n\n", ifname);

    free(buffer);
    return foundNetworks;
}

inline WifiNetwork*
RetrieveWifiNetworkInfo(struct stream_descr *	stream,	/* Stream of events */
                  struct iw_event *		event,	/* Extracted token */
                  struct iwscan_state *	state,
                  struct iw_range *	iw_range,	/* Range info */
                  int		has_range,
                        WifiNetwork* lastFoundWifiNetwork)
{
    //char		buffer[128];	/* Temporary buffer */
    WifiNetwork* foundNetwork = NULL;

    /* Now, let's decode the event */
    switch(event->cmd)
    {
    case SIOCGIWAP:
        //printf("Sinal %02d - Qualidade: ", state->ap_num,
        //       iw_saether_ntop(&event->u.ap_addr, buffer));
        state->ap_num++;
        break;
        //    case SIOCGIWNWID:
        //        if(event->u.nwid.disabled)
        //            printf("                    NWID:off/any\n");
        //        else
        //            printf("                    NWID:%X\n", event->u.nwid.value);
        //        break;
        //    case SIOCGIWFREQ:
        //    {
        //        double		freq;			/* Frequency/channel */
        //        int		channel = -1;		/* Converted to channel */
        //        freq = iw_freq2float(&(event->u.freq));
        //        /* Convert to channel if possible */
        //        if(has_range)
        //            channel = iw_freq_to_channel(freq, iw_range);
        //        iw_print_freq(buffer, sizeof(buffer),
        //                      freq, channel, event->u.freq.flags);
        //        printf("                    %s\n", buffer);
        //    }
        //        break;
        //    case SIOCGIWMODE:
        //        /* Note : event->u.mode is unsigned, no need to check <= 0 */
        //        if(event->u.mode >= IW_NUM_OPER_MODE)
        //            event->u.mode = IW_NUM_OPER_MODE;
        //        printf("                    Mode:%s\n",
        //               iw_operation_mode[event->u.mode]);
        //        break;
        //    case SIOCGIWNAME:
        //        printf("                    Protocol:%-1.16s\n", event->u.name);
        //        break;
    case SIOCGIWESSID:
    {
        foundNetwork = new WifiNetwork();
        char essid[4*IW_ESSID_MAX_SIZE+1];
        memset(essid, '\0', sizeof(essid));
        if((event->u.essid.pointer) && (event->u.essid.length))
            iw_essid_escape(essid, static_cast<const char*>(event->u.essid.pointer), event->u.essid.length);
        if(event->u.essid.flags)
        {
            foundNetwork->SSID = string(essid);
            /* Does it have an ESSID index ? */
            //if((event->u.essid.flags & IW_ENCODE_INDEX) > 1)
            //    printf("    ESSID:\"%s\" [%d]\n", essid,
            //           (event->u.essid.flags & IW_ENCODE_INDEX));
            //else
            //    printf("    ESSID:\"%s\"\n", essid);
        }
        else
            foundNetwork->SSID = "";
            //printf("    ESSID:off/any/hidden\n");
    }
        break;
        //    case SIOCGIWENCODE:
        //    {
        //        unsigned char	key[IW_ENCODING_TOKEN_MAX];
        //        if(event->u.data.pointer)
        //            memcpy(key, event->u.data.pointer, event->u.data.length);
        //        else
        //            event->u.data.flags |= IW_ENCODE_NOKEY;
        //        printf("                    Encryption key:");
        //        if(event->u.data.flags & IW_ENCODE_DISABLED)
        //            printf("off\n");
        //        else
        //        {
        //            /* Display the key */
        //            iw_print_key(buffer, sizeof(buffer), key, event->u.data.length,
        //                         event->u.data.flags);
        //            printf("%s", buffer);

        //            /* Other info... */
        //            if((event->u.data.flags & IW_ENCODE_INDEX) > 1)
        //                printf(" [%d]", event->u.data.flags & IW_ENCODE_INDEX);
        //            if(event->u.data.flags & IW_ENCODE_RESTRICTED)
        //                printf("   Security mode:restricted");
        //            if(event->u.data.flags & IW_ENCODE_OPEN)
        //                printf("   Security mode:open");
        //            printf("\n");
        //        }
        //    }
        //        break;
        //    case SIOCGIWRATE:
        //        if(state->val_index == 0)
        //            printf("                    Bit Rates:");
        //        else
        //            if((state->val_index % 5) == 0)
        //                printf("\n                              ");
        //            else
        //                printf("; ");
        //        iw_print_bitrate(buffer, sizeof(buffer), event->u.bitrate.value);
        //        printf("%s", buffer);
        //        /* Check for termination */
        //        if(stream->value == NULL)
        //        {
        //            printf("\n");
        //            state->val_index = 0;
        //        }
        //        else
        //            state->val_index++;
        //        break;
        //    case SIOCGIWMODUL:
        //    {
        //        unsigned int	modul = event->u.param.value;
        //        int		i;
        //        int		n = 0;
        //        printf("                    Modulations :");
        //        for(i = 0; i < IW_SIZE_MODUL_LIST; i++)
        //        {
        //            if((modul & iw_modul_list[i].mask) == iw_modul_list[i].mask)
        //            {
        //                if((n++ % 8) == 7)
        //                    printf("\n                        ");
        //                else
        //                    printf(" ; ");
        //                printf("%s", iw_modul_list[i].cmd);
        //            }
        //        }
        //        printf("\n");
        //    }
        //        break;
    case IWEVQUAL:
        //print_event_quality_signal_level(buffer, sizeof(buffer),
        //                                 &event->u.qual, iw_range, has_range);
        //printf("%s\n", buffer);
        break;
#ifndef WE_ESSENTIAL
            case IWEVGENIE:
                /* Informations Elements are complex, let's do only some of them */
                lastFoundWifiNetwork->TSMACodedMessages = RetrieveStuffedTSMAMessages(static_cast<unsigned char*>(event->u.data.pointer), event->u.data.length);
                break;
#endif	/* WE_ESSENTIAL */
//            case IWEVCUSTOM:
//            {
//                char custom[IW_CUSTOM_MAX+1];
//                if((event->u.data.pointer) && (event->u.data.length))
//                    memcpy(custom, event->u.data.pointer, event->u.data.length);
//                custom[event->u.data.length] = '\0';
//                printf("                    Extra:%s\n", custom);
//            }
//                break;
//            default:
//                printf("                    (Unknown Wireless Token 0x%04X)\n",
//                       event->cmd);
    }	/* switch(event->cmd) */
    return foundNetwork;
}

// Método Adaptado da Duplicação do Iwlist.c -> iw_print_gen_ie
inline list<string> RetrieveStuffedTSMAMessages(unsigned char* buffer, int	buflen)
{
    int offset = 0;
    const int vendorInfoIdentifier = 0xdd;
    unsigned char stuffedMessageIdentifier[2] = {0x56, 0xaf};

    list<string> vendorInfoList;
    /* Loop on each IE, each IE is minimum 2 bytes */
    while(offset <= (buflen - 2))
    {
        int ieLength = buffer[offset + 1] + 2; // ("dd" + lenght = 2)
        /* Not all IEs that start with 0xdd are WPA. * So check that the OUI is valid. Note : offset==2 */

        if (ieLength > 3) // Version (1) + TSMA ID (2)
        {
            bool isVendorIE = (buffer[offset] == vendorInfoIdentifier);
            bool isStuffedMessage = (memcmp(&buffer[offset+3], stuffedMessageIdentifier, 2) == 0); //((ieLength < 8) || (memcmp(&buffer[offset+2], wpa1_oui, 3) != 0) || (buffer[offset + 3] != 0x01));
            if (isVendorIE && isStuffedMessage) {
                int startIdx = offset + 2;
                int endIdx = offset + ieLength;

                ostringstream vendorInfoStream;
                for (int i = startIdx; i < endIdx; ++i)
                    vendorInfoStream << std::hex << std::setfill('0') << std::setw(2) << (int)buffer[i];

               string vendorInfoData = vendorInfoStream.str();
               std::transform(vendorInfoData.begin(), vendorInfoData.end(), vendorInfoData.begin(), ::toupper);

               vendorInfoList.push_back(vendorInfoData);
            }
        }

        /* Skip over this IE to the next one in the list. */
        offset += ieLength;
    }
    return vendorInfoList;
}

// Método Adaptado da Duplicação do Iwlist.c -> iw_print_gen_ie
static inline list<string> RetrieveVendorIEs(unsigned char* buffer, int	buflen)
{
    int offset = 0;
    const int VendorInfoIdentifier = 0xdd;
    unsigned char wpa1_oui[3] = {0x00, 0x50, 0xf2};

    list<string> vendorInfoList;
    /* Loop on each IE, each IE is minimum 2 bytes */
    while(offset <= (buflen - 2))
    {
        int ieLength = buffer[offset + 1] + 2;
        /* Not all IEs that start with 0xdd are WPA. * So check that the OUI is valid. Note : offset==2 */

        bool isVendorIEorWpaIE = (buffer[offset] == VendorInfoIdentifier);
        bool isNotWpaIE = (memcmp(&buffer[offset+2], wpa1_oui, 3) != 0); //((ieLength < 8) || (memcmp(&buffer[offset+2], wpa1_oui, 3) != 0) || (buffer[offset + 3] != 0x01));
        if (isVendorIEorWpaIE && isNotWpaIE) {
            int startIdx = offset;
            int endIdx = offset + ieLength;

            ostringstream vendorInfoStream;
            for (int i = startIdx; i < endIdx; ++i)
                vendorInfoStream << std::hex << std::setfill('0') << std::setw(2) << (int)buffer[i];

            string vendorInfoData = vendorInfoStream.str();
            std::transform(vendorInfoData.begin(), vendorInfoData.end(), vendorInfoData.begin(), ::toupper);

            vendorInfoList.push_back(vendorInfoData);
        }

        /* Skip over this IE to the next one in the list. */
        offset += ieLength;
    }
    return vendorInfoList;
}
