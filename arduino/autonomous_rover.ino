/*
 * Autonomous Obstacle Avoidance Rover
 *
 * Controller : Arduino Uno
 * Sensor     : HC-SR04 ultrasonic sensor
 * Scanner    : SG90 servo
 * Driver     : L298N
 * Motors     : 4x BO geared DC motors
 *
 * Operation:
 * 1. Move forward while the path is clear.
 * 2. If an obstacle is detected, stop.
 * 3. Scan left and right using the servo-mounted HC-SR04.
 * 4. Select the direction with greater clearance.
 * 5. Turn and continue forward.
 */

#include <Servo.h>

// ============================================================
// Pin Configuration
// ============================================================

// HC-SR04
const int TRIG_PIN = 2;
const int ECHO_PIN = 3;

// L298N - Left motor pair
const int ENA = 5;
const int IN1 = 7;
const int IN2 = 8;

// L298N - Right motor pair
const int ENB = 6;
const int IN3 = 11;
const int IN4 = 12;

// Servo
const int SERVO_PIN = 9;

// ============================================================
// Rover Configuration
// ============================================================

const int MOTOR_SPEED = 180;

// Distance at which obstacle avoidance begins
const int OBSTACLE_THRESHOLD = 25;

// Maximum useful ultrasonic measurement
const int MAX_DISTANCE = 200;

// Servo positions
const int SERVO_CENTER = 90;
const int SERVO_LEFT   = 150;
const int SERVO_RIGHT  = 30;

// Time required for servo to reach scan position
const int SERVO_SETTLE_TIME = 400;

// Turning duration
// Adjust experimentally according to the physical chassis.
const int TURN_TIME = 650;

// Short delay after stopping
const int STOP_DELAY = 150;

Servo scannerServo;

// ============================================================
// Setup
// ============================================================

void setup()
{
    Serial.begin(9600);

    // Ultrasonic sensor
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    // Motor driver
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);

    pinMode(ENB, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    // Servo
    scannerServo.attach(SERVO_PIN);
    scannerServo.write(SERVO_CENTER);

    // Make sure motors start stopped
    stopMotors();

    delay(1000);

    Serial.println("=================================");
    Serial.println(" Autonomous Obstacle Avoidance");
    Serial.println(" Rover");
    Serial.println("=================================");
    Serial.println("System initialized.");

    // Initial forward-facing measurement
    long distance = measureDistance();

    Serial.print("Initial distance: ");
    Serial.print(distance);
    Serial.println(" cm");
}

// ============================================================
// Main Loop
// ============================================================

void loop()
{
    // Keep sensor facing forward during normal operation
    scannerServo.write(SERVO_CENTER);

    long frontDistance = measureDistance();

    Serial.print("Front: ");
    Serial.print(frontDistance);
    Serial.println(" cm");

    // --------------------------------------------------------
    // Path clear
    // --------------------------------------------------------

    if (frontDistance > OBSTACLE_THRESHOLD)
    {
        moveForward();
        delay(50);
        return;
    }

    // --------------------------------------------------------
    // Obstacle detected
    // --------------------------------------------------------

    Serial.println("Obstacle detected!");

    stopMotors();
    delay(STOP_DELAY);

    // Scan both directions
    long leftDistance = scanLeft();
    long rightDistance = scanRight();

    Serial.print("Left: ");
    Serial.print(leftDistance);
    Serial.println(" cm");

    Serial.print("Right: ");
    Serial.print(rightDistance);
    Serial.println(" cm");

    // Return sensor to center
    scannerServo.write(SERVO_CENTER);
    delay(SERVO_SETTLE_TIME);

    // --------------------------------------------------------
    // Select direction
    // --------------------------------------------------------

    if (leftDistance > rightDistance)
    {
        Serial.println("Decision: TURN LEFT");

        turnLeft();
        delay(TURN_TIME);
    }
    else
    {
        Serial.println("Decision: TURN RIGHT");

        turnRight();
        delay(TURN_TIME);
    }

    stopMotors();
    delay(STOP_DELAY);
}

// ============================================================
// Ultrasonic Distance Measurement
// ============================================================

long measureDistance()
{
    // Ensure clean trigger pulse
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Timeout prevents the rover from getting stuck
    // if no echo is received.
    unsigned long duration = pulseIn(ECHO_PIN, HIGH, 25000);

    if (duration == 0)
    {
        return MAX_DISTANCE;
    }

    // Speed of sound ≈ 0.0343 cm/us
    long distance = duration * 0.0343 / 2;

    // Limit the returned value
    if (distance > MAX_DISTANCE)
        distance = MAX_DISTANCE;

    if (distance < 2)
        distance = 2;

    return distance;
}

// ============================================================
// Servo Scanning
// ============================================================

long scanLeft()
{
    Serial.println("Scanning LEFT...");

    scannerServo.write(SERVO_LEFT);
    delay(SERVO_SETTLE_TIME);

    return measureDistance();
}

long scanRight()
{
    Serial.println("Scanning RIGHT...");

    scannerServo.write(SERVO_RIGHT);
    delay(SERVO_SETTLE_TIME);

    return measureDistance();
}

// ============================================================
// Motor Control
// ============================================================

void moveForward()
{
    // Left motor pair
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    // Right motor pair
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

    analogWrite(ENA, MOTOR_SPEED);
    analogWrite(ENB, MOTOR_SPEED);
}

void moveBackward()
{
    // Left motor pair
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);

    // Right motor pair
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);

    analogWrite(ENA, MOTOR_SPEED);
    analogWrite(ENB, MOTOR_SPEED);
}

void stopMotors()
{
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);

    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}

// ============================================================
// Turning
// ============================================================

void turnLeft()
{
    Serial.println("Turning LEFT");

    // Left wheels backward
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);

    // Right wheels forward
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

    analogWrite(ENA, MOTOR_SPEED);
    analogWrite(ENB, MOTOR_SPEED);
}

void turnRight()
{
    Serial.println("Turning RIGHT");

    // Left wheels forward
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    // Right wheels backward
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);

    analogWrite(ENA, MOTOR_SPEED);
    analogWrite(ENB, MOTOR_SPEED);
}
