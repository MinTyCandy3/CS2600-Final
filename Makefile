
pub: mqtt_pub.c
	gcc mqtt_pub.c -o mqtt_pub -lpaho-mqtt3c

sub: mqtt_sub.c
	gcc mqtt_sub.c -o mqtt_sub -lpaho-mqtt3c

runpub: mqtt_pub
	./mqtt_pub

runsub: mqtt_sub
	./mqtt_sub
