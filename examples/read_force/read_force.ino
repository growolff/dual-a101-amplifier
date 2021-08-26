/*
    
*/

#define LEFT_PIN A0  // Analog input pin that the left sensor is attached to
#define RIGHT_PIN A1

int l_val = 0;        // value read from the sensor
int r_val = 0;

float l_force = 0;
float r_force = 0;

float c_factor = 0.2908;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // read ADC values
  l_val = smooth(LEFT_PIN);
  r_val = smooth(RIGHT_PIN);

  // converts the voltaje value to force unit [kg] (see calibration guide)
  l_force = l_val * 5.0/1024.0*c_factor; 
  r_force = r_val * 5.0/1024.0*c_factor;

  // using the ecuation for equilibrum state
  float Df = 0.017; // distance in meters
  float de = 0.0085;

  float alpha = atan(0.003/de) * 3.14159 / 180; // in radians
  float beta = atan(0.003/Df) * 3.14159 / 180; // in radians

  float l_tens = Df*l_force*cos(alpha) / (de * cos(beta));
  float r_tens = Df*r_force*cos(alpha) / (de * cos(beta));

  // formatea la info para mostrarla en el serial plotter
  Serial.print("l_flex: ");
  Serial.print(l_tens*1000); 
  Serial.print("\tr_flex: ");
  Serial.print(l_force*1000);
  
  Serial.println("");
  
  delay(10);
}

int smooth(const int sensor){
  int i;
  int value = 0;
  int numReadings = 10;

  for (i = 0; i < numReadings; i++){
    // Read light sensor data.
    value = value + analogRead(sensor);

    // 1ms pause adds more stability between reads.
    delay(1);
  }

  // Take an average of all the readings.
  value = value / numReadings;

  // Scale to 8 bits (0 - 255).
  // value = value / 4;

  return value;
}
