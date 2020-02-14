void setup() {
  Serial.begin(9600);

}

void loop() {
  double user_defined_Lat = 10.297543;
  double user_defined_Lang = 123.896834;
  double user_defined_radius = 0.0003;
  double latitude = 10.297459;
  double longitude = 123.896567;

  double latOffset = user_defined_Lat - latitude;
  double lngOffset = user_defined_Lang - longitude;
  if( ((latOffset <= user_defined_radius) && (latOffset >= (user_defined_radius * -1))) && ((lngOffset <= user_defined_radius) && (lngOffset >= (user_defined_radius * -1))) )
  {
    Serial.println("Naa sa circle");                                              
  }  
  else {
    Serial.println("Wala sa circle");
  }
  delay(1000);
}
