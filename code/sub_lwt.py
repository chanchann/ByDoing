import paho.mqtt.client as mqtt

def on_connect(client,usredata,flags,rc):
    print("Connedted with result code "+str(rc))
    client.subscribe("test/resanning/#")

def on_message(client,userdata,msg):
    print(msg.topic+" "+str(msg.payload))

client=mqtt.Client()
client.on_connect=on_connect
client.on_message=on_message
client.connect("localhost",1883)
client.loop_forever()