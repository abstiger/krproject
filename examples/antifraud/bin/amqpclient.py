#!/usr/bin/env python
#coding=utf-8
# vim: set ts=4 sw=4 et:
"""
Example of krengine amqp client
"""
import sys
import time
import json

# Detect if we're running in a git repo
from os.path import exists, normpath
if exists(normpath('../pika')):
    sys.path.insert(0, '..')

from pika.adapters import SelectConnection
from pika.connection import ConnectionParameters
from pika import BasicProperties
import pika.log

# We use these to hold our connection & channel
connection = None
channel = None


def on_connected(connection):
    print "demo_send: Connected to RabbitMQ"
    connection.channel(on_channel_open)


def on_channel_open(channel_):
    global channel
    channel = channel_
    print "demo_send: Received our Channel"
    channel.queue_declare(queue="krqueue", durable=True,
                          exclusive=False, auto_delete=False,
                          callback=on_queue_declared)


def on_queue_declared(frame):
    print "demo_send: Queue Declared"
    for x in xrange(0, 2):
        
        # Construct request message with json format
        msg = {}
        msg['cust_no'] = "6223000000000000%04i" % (x%10)
        msg['trans_date'] = "201205%02i" % (x%7)
        msg['trans_time'] = "0919%02i" % (x%60)
        msg['flow_no'] = "%015i" % x
        msg['trans_type'] = "%02i" % (x%2)
        msg['trans_amt'] = x*9.9
        msg['trans_loc'] = "trans location:%02i" % x
        jsonmsg = json.dumps(msg, indent=2)

        # Create properties with when we sent the message, the app_id
        # user we connected with, a content type and non persisted messages
        properties = BasicProperties(timestamp=time.time(),
                                     app_id=__file__,
                                     user_id='guest',
                                     content_type="text/plain",
                                     delivery_mode=1)

        # Send the message
        channel.basic_publish(exchange='',
                              routing_key="krqueue",
                              body=jsonmsg,
                              properties=properties)

        print "demo_send:" + jsonmsg

    # Close our connection
    print "Closing client"
    connection.close()


if __name__ == '__main__':
    pika.log.setup(level=pika.log.INFO)

    # Connect to RabbitMQ
    host = (len(sys.argv) > 1) and sys.argv[1] or '127.0.0.1'
    connection = SelectConnection(ConnectionParameters(host),
                                  on_connected)
    # Loop until CTRL-C
    try:
        # Start our blocking loop
        connection.ioloop.start()

    except KeyboardInterrupt:

        # Close the connection
        connection.close()

        # Loop until the connection is closed
        connection.ioloop.stop()
