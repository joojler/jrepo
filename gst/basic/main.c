
/*
 * 1. how to print guint64
 * 2. GstClockTime == gunit64
 */

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include <gst/gst.h>

typedef struct _CustomData {
  guint sourceid;        /* To control the GSource */
  GstElement *appsrc;
} CustomData;


static GMainLoop *loop;

static gboolean my_bus_callback(GstBus *bus,
				GstMessage *msg,
				gpointer data)
{
	GMainLoop *loop = (GMainLoop *)data;
	
	switch (GST_MESSAGE_TYPE(msg)) {
		case GST_MESSAGE_EOS:
			g_print("End of stream\n");
			g_main_loop_quit(loop);
			break;
	
		case GST_MESSAGE_ERROR: {
			gchar *debug;
			GError *error;
			
			gst_message_parse_error(msg, &error, &debug);
			g_free(debug);

			g_printerr("Error: %s\n", error->message);
			g_error_free(error);
			
			g_main_loop_quit(loop);
			break;
		}

		default:
			break;
	}

	return TRUE;
}


static void
cb_need_data (GstElement *appsrc,
	      guint       unused_size,
	      gpointer    user_data)
{
  static gboolean white = FALSE;
  static GstClockTime timestamp = 0;
  GstBuffer *buffer;
  guint size;
  GstFlowReturn ret;

  //g_print ("cb_need_data called!\n");
//  sleep (1);
  size = 385 * 288 * 2;

  buffer = gst_buffer_new_allocate (NULL, size, NULL);

  /* this makes the image black/white */
  gst_buffer_memset (buffer, 0, white ? 0xff : 0x0, size);
  
  white = !white;

//  GST_BUFFER_PTS (buffer) = timestamp;
//  GST_BUFFER_DURATION (buffer) = gst_util_uint64_scale_int (1, GST_SECOND, 2);

// timestamp += GST_BUFFER_DURATION (buffer);

  g_signal_emit_by_name (appsrc, "push-buffer", buffer, &ret);

  if (ret != GST_FLOW_OK) {
    /* something wrong, stop pushing */
    g_main_loop_quit (loop);
  }
}

static gboolean push_data (CustomData *data) {
  static gboolean white = FALSE;
  static GstClockTime timestamp = 0;
  GstBuffer *buffer;
  guint size;
  GstFlowReturn ret;

//  g_print ("cb_need_data called!\n");

#if 1
  size = 385 * 288 * 2;

  buffer = gst_buffer_new_allocate (NULL, size, NULL);

  /* this makes the image black/white */
  gst_buffer_memset (buffer, 0, white ? 0xff : 0x0, size);
  
  white = !white;

//  GST_BUFFER_PTS (buffer) = timestamp;
//  GST_BUFFER_DURATION (buffer) = gst_util_uint64_scale_int (1, GST_SECOND, 2);

  timestamp += GST_BUFFER_DURATION (buffer);
  g_print("uint64 timestamp: %" PRIu64 "\n", timestamp);
  g_signal_emit_by_name (data->appsrc, "push-buffer", buffer, &ret);

  if (ret != GST_FLOW_OK) {
    /* something wrong, stop pushing */
    g_main_loop_quit (loop);
  }
#endif
  return TRUE;
}
   
/* This signal callback triggers when appsrc needs data. Here, we add an idle handler
 * to the mainloop to start pushing data into the appsrc */
static void start_feed (GstElement *source, guint size, CustomData *data) {
  if (data->sourceid == 0) {
    g_print ("Start feeding\n");
	data->appsrc = source;
    data->sourceid = g_idle_add ((GSourceFunc) push_data, data);
  }
}
   
/* This callback triggers when appsrc has enough data and we can stop sending.
 * We remove the idle handler from the mainloop */
static void stop_feed (GstElement *source, CustomData *data) {
  if (data->sourceid != 0) {
    g_print ("Stop feeding\n");
    g_source_remove (data->sourceid);
    data->sourceid = 0;
  }
}


gint
main (gint   argc,
      gchar *argv[])
{
  GstElement *pipeline, *pl2, *appsrc, *conv, *videosink, *h264parse;
  GError *error = NULL;
  CustomData data;


  memset (&data, 0, sizeof (CustomData));
  /* init GStreamer */
  gst_init (&argc, &argv);
  loop = g_main_loop_new (NULL, FALSE);

  //pl2 = gst_parse_launch ("videotestsrc ! video/x-raw,width=352,height=255 ! xvimagesink", &error);

  /* setup pipeline */
  pipeline = gst_pipeline_new ("pipeline");
  appsrc = gst_element_factory_make ("appsrc", "source");
  h264parse = gst_element_factory_make ("h264parse", "parse");
  conv = gst_element_factory_make ("videoconvert", "conv");
  videosink = gst_element_factory_make ("xvimagesink", "videosink");

  /* setup */
  g_object_set (G_OBJECT (appsrc), "caps",
  		gst_caps_new_simple ("video/x-raw",
				     "format", G_TYPE_STRING, "RGB16",
				     "width", G_TYPE_INT, 384,
				     "height", G_TYPE_INT, 288,
				     "framerate", GST_TYPE_FRACTION, 10, 1,
					 "block", G_TYPE_BOOLEAN, TRUE,
					 "max-bytes", G_TYPE_INT, 2*1024*1024,
				     NULL), NULL);
  gst_bin_add_many (GST_BIN (pipeline), appsrc, h264parse, conv, videosink, NULL);
  gst_element_link_many (appsrc, h264parse, conv, videosink, NULL);

  /* setup appsrc */
  g_object_set (G_OBJECT (appsrc),
		"stream-type", 0,
		"format", GST_FORMAT_TIME, NULL);
  g_signal_connect (appsrc, "need-data", G_CALLBACK (cb_need_data), NULL);
//  g_signal_connect (appsrc, "need-data", G_CALLBACK (start_feed), &data);
//  g_signal_connect (appsrc, "enough-data", G_CALLBACK (stop_feed), &data);

  /* play */
  gst_element_set_state (pipeline, GST_STATE_PLAYING);
//  gst_element_set_state (pl2, GST_STATE_PLAYING);
	gst_bus_add_watch ((GST_ELEMENT_BUS (pipeline)),
					my_bus_callback, 
					loop);
  g_main_loop_run (loop);

  /* clean up */
  gst_element_set_state (pipeline, GST_STATE_NULL);
  gst_object_unref (GST_OBJECT (pipeline));
  g_main_loop_unref (loop);

  return 0;
  }
