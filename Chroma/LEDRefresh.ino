int lastRefresh=0;

void refreshLED() {

  if (1000/60 < millis()) {
    /*Perform refresh*/
    lastRefresh = millis();



    /*Display changes*/
    FastLED.show();

  }

}
