#include <OfflineLogger.h>
#include <Ornament.h>

OfflineLogger logger = OfflineLogger("test", &Serial, true);

Ornament ornament = Ornament(8, &logger);

float start_sec;

void setup()
{
    Serial.begin(9600);
    logger.log("start");
}

void loop()
{
  delay(1000);
  logger.log(String(ornament.test()));

}
