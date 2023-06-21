#include <painlessMesh.h>
#include <AsyncTCP.h>
#define LED 2 
#define BLINK_PERIOD 3000 
#define BLINK_DURATION 100 
#define MESH_SSID "ESP32 Remote Lab"
#define MESH_PASSWORD "12345678"
#define MESH_PORT 5555
int SensorPin = 32;// deklarasi pin analog yg dipakai
int soilMoistureValue; // menyimpan nilai analog dari sensor ke esp32
int soilmoisturepercent; // nilai yg diperoleh dalam bentuk persen setelah dimaping
// Prototypes
void sendMessage(); 

void receivedCallback(uint32_t from, String & msg);
void newConnectionCallback(uint32_t nodeId);
void changedConnectionCallback(); 
void nodeTimeAdjustedCallback(int32_t offset); 
void delayReceivedCallback(uint32_t from, int32_t delay);
Scheduler userScheduler; // to control your personal task
painlessMesh mesh;
bool calc_delay = false;
SimpleList<uint32_t> nodes;
void sendMessage() ; // Prototype
Task taskSendMessage( TASK_SECOND * 1, TASK_FOREVER, 
&sendMessage ); // start with a one second interval
// Task to blink the number of nodes
Task blinkNoNodes;
bool onFlag = false;
void setup() {
 Serial.begin(9600);
 pinMode(LED, OUTPUT);
 mesh.setDebugMsgTypes(ERROR | DEBUG); // set before init() so that you can see error messages
 mesh.init(MESH_SSID, MESH_PASSWORD, &userScheduler, MESH_PORT);
 mesh.onReceive(&receivedCallback);
 mesh.onNewConnection(&newConnectionCallback);
 mesh.onChangedConnections(&changedConnectionCallback);
 mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
 mesh.onNodeDelayReceived(&delayReceivedCallback);
 userScheduler.addTask( taskSendMessage );
 taskSendMessage.enable();
 blinkNoNodes.set(BLINK_PERIOD, (mesh.getNodeList().size() + 1) * 2, []() {
 // If on, switch off, else switch on

 if (onFlag)
 onFlag = false;
 else
 onFlag = true;
 blinkNoNodes.delay(BLINK_DURATION);
 if (blinkNoNodes.isLastIteration()) {
 // Finished blinking. Reset task for next run 
 // blink number of nodes (including this node) times
 blinkNoNodes.setIterations((mesh.getNodeList().size() + 1) * 2);
 // Calculate delay based on current mesh time and 
 BLINK_PERIOD;
 // This results in blinks between nodes being synced
 blinkNoNodes.enableDelayed (BLINK_PERIOD - (mesh.getNodeTime() % (BLINK_PERIOD*1000))/1000);
 }
 });
 userScheduler.addTask(blinkNoNodes);
 blinkNoNodes.enable();
 randomSeed(analogRead(A0));
}
void loop() {
 mesh.update();
 digitalWrite(LED, !onFlag);
}
void sendMessage() {
  // Read the humidity from the sensor.
      soilMoistureValue = analogRead(SensorPin);
        Serial.print("Nilai analog = ");
          Serial.print(soilMoistureValue);
           soilmoisturepercent = map(soilMoistureValue, 4095, 0, 0, 100);

        Serial.print(" Presentase kelembaban tanah= ");
       Serial.print(soilmoisturepercent);
      Serial.println("% ");
 String msg = "Hello from node 2";
 msg += mesh.getNodeId();
 msg += " myFreeMemory: " + String(ESP.getFreeHeap());
 mesh.sendBroadcast(msg);
 if (calc_delay) {
 SimpleList<uint32_t>::iterator node = nodes.begin();
 while (node != nodes.end()) {
 mesh.startDelayMeas(*node);

 node++;
 }
 calc_delay = false;
 }
 Serial.printf("Sending message: %s\n", msg.c_str());
 taskSendMessage.setInterval( random(TASK_SECOND * 1, TASK_SECOND * 5)); // between 1 and 5 seconds
}
void receivedCallback(uint32_t from, String & msg) {
 Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}
void newConnectionCallback(uint32_t nodeId) {
 // Reset blink task
 onFlag = false;
 blinkNoNodes.setIterations((mesh.getNodeList().size() + 1) * 2);
 blinkNoNodes.enableDelayed(BLINK_PERIOD -
(mesh.getNodeTime() % (BLINK_PERIOD*1000))/1000);
 Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
 Serial.printf("--> startHere: New Connection, %s\n", mesh.subConnectionJson(true).c_str());
}
void changedConnectionCallback() {
 Serial.printf("Changed connections\n");
 // Reset blink task
 onFlag = false;
 blinkNoNodes.setIterations((mesh.getNodeList().size() + 1) * 2);
 blinkNoNodes.enableDelayed(BLINK_PERIOD - (mesh.getNodeTime() % (BLINK_PERIOD*1000))/1000);

 nodes = mesh.getNodeList();
 Serial.printf("Num nodes: %d\n", nodes.size());
 Serial.printf("Connection list:");
 SimpleList<uint32_t>::iterator node = nodes.begin();
 while (node != nodes.end()) {
 Serial.printf(" %u", *node);
 node++;
 }
 Serial.println();
 calc_delay = true;
}
void nodeTimeAdjustedCallback(int32_t offset) {
 Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}
void delayReceivedCallback(uint32_t from, int32_t delay) {
 Serial.printf("Delay to node %u is %d us\n", from, delay);
}