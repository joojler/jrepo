
#!/bin/bash
# 1. for loop with new defined variable
# 2. how to use x-tile to tiling all opened window


CH_NUM=16

#gst-launch-1.0 filesrc location=/mnt/H264_High@L4.0_1920x1080_5Mbps_30.0fps_Interlaced_Sound_Football.mp4 ! qtdemux ! vaapidecode ! vaapisink sync=true &

for ((i=0; i<$CH_NUM; i++)) 
do
	echo "stream $i opened..."
	gst-launch-1.0 filesrc location=/mnt/H264_High@L4.0_1920x1080_5Mbps_30.0fps_Interlaced_Sound_Football.mp4 ! qtdemux ! vaapidecode ! vaapipostproc width=384 height=270 ! vaapisink sync=true  use-glx=true &
done

x-tile g 4 4
