import paho.mqtt.client as mqtt
import time

a=0
def on_connect(client,userdata,flags,rc):
    print("Connected with result code "+str(rc))

def on_message(client,userdata,msg):1    
    print(msg.topic+" "+str(msg.payload))

client=mqtt.Client()
client.on_connect=on_connect
client.on_message=on_message
client.connect("192.168.199.241",1884)

while client.loop()==0:
    msg="test message from Publisher "+time.ctime()
    client.publish("chanchan",a,True)
    print(a)
    a=a+1
    time.sleep(1.5)
    pass
    