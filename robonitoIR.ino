/*
 * IRobonito V2
 * IR remote controlled Robot
 * (c) 2020 Hernan Moya
 *
 * This sketch controls a robot using an IR remote. It defines actions such as
 * moving forward, moving backward, turning left, turning right, and stopping.
 * It listens for specific IR remote codes to trigger these actions.
 * 
 * REQUIRED LIBRARY: IRremote by shirriff (tested with v3.x)
 * Install via Arduino Library Manager: Search for "IRremote"
 * 
 * TROUBLESHOOTING:
 * - If robot doesn't respond, verify IR codes match your remote using Serial Monitor
 * - To capture your remote's codes: Upload this sketch, press buttons, note values in Serial Monitor
 * - Update the IR_CODE_* constants below with your remote's actual codes
 */

#include <Servo.h>
#include <IRremote.h>

// Pin definitions
#define RECV_PIN 4
#define SERVO_RIGHT_PIN 8
#define SERVO_LEFT_PIN 9

// IR Remote Button Codes (NEC protocol - update these for your remote!)
#define IR_CODE_FORWARD  839727
#define IR_CODE_STOP     803007
#define IR_CODE_LEFT     831567
#define IR_CODE_RIGHT    815247
#define IR_CODE_BACK     798927

// Debounce delay in milliseconds (prevents multiple rapid triggers)
#define IR_DEBOUNCE_DELAY 200

// Set to 0 to disable serial output for better performance
#define DEBUG_SERIAL 1

// Servo objects
Servo servoRight;
Servo servoLeft;

// IR receiver object
IRrecv irrecv(RECV_PIN);
decode_results results;

// State tracking - current movement state
enum RobotState { STOPPED, MOVING_FORWARD, MOVING_BACK, TURNING_LEFT, TURNING_RIGHT };
RobotState currentState = STOPPED;
unsigned long lastIrCommandTime = 0;

/*
 * Function declarations for robot actions
 */
void moveForward();
void moveBack();
void turnRight();
void turnLeft();
void stopMoving();

void setup()
{
  Serial.begin(9600);
  
  // Attach servos with error checking
  servoRight.attach(SERVO_RIGHT_PIN);
  servoLeft.attach(SERVO_LEFT_PIN);
  
#if DEBUG_SERIAL
  Serial.println("IRbonito V2 (c) 2020");
  Serial.println("IRbonito: Ready to start...");
#endif
  
  irrecv.enableIRIn(); // Start the IR receiver
  stopMoving(); // Ensure robot starts stopped
}

void loop()
{
  // Check for IR remote commands (non-blocking)
  if (irrecv.decode(&results)) {
    
    // Apply debounce delay
    if (millis() - lastIrCommandTime > IR_DEBOUNCE_DELAY) {
      lastIrCommandTime = millis();
      
#if DEBUG_SERIAL
      Serial.println(results.value, DEC);
#endif

      // Check IR remote codes and perform actions accordingly
      if (results.value == IR_CODE_FORWARD) {
        moveForward();
      } else if (results.value == IR_CODE_STOP) {
        stopMoving();
      } else if (results.value == IR_CODE_LEFT) {
        turnLeft();
      } else if (results.value == IR_CODE_RIGHT) {
        turnRight();
      } else if (results.value == IR_CODE_BACK) {
        moveBack();
      }
    }

    irrecv.resume(); // Receive the next value
  }
  
  // Small delay to prevent CPU hogging
  delay(10);
}

void moveForward()
{
  servoLeft.write(180);
  servoRight.write(0);
#if DEBUG_SERIAL
  Serial.println("IRbonito: FORWARD");
#endif
  currentState = MOVING_FORWARD;
}

void moveBack()
{
  servoLeft.write(0);
  servoRight.write(180);
#if DEBUG_SERIAL
  Serial.println("IRbonito: BACK");
#endif
  currentState = MOVING_BACK;
}

void turnRight()
{
  servoLeft.write(180);
  servoRight.write(180);
#if DEBUG_SERIAL
  Serial.println("IRbonito: RIGHT");
#endif
  currentState = TURNING_RIGHT;
}

void turnLeft()
{
  servoLeft.write(0);
  servoRight.write(0);
#if DEBUG_SERIAL
  Serial.println("IRbonito: LEFT");
#endif
  currentState = TURNING_LEFT;
}

void stopMoving()
{
  servoLeft.write(90);
  servoRight.write(90);
#if DEBUG_SERIAL
  Serial.println("IRbonito: STOP");
#endif
  currentState = STOPPED;
}
