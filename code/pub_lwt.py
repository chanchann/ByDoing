import paho.mqtt.client as mqtt
import time

def on_connect(client,userdata,flags,rc):
    print("Connected with result code "+str(rc))

client=mqtt.Client()
client.on_connect=on_connect
client.will_set('test/rensanning/will','Last will message',0,False)
client.connect("localhost",1883)

client.loop_start()

msg="test LWT"+time.ctime()
client.publish("test/resanning/time",msg,0)
print("message published")

client.loop_stop()

print "done"