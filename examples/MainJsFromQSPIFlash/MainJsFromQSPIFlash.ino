#include "quickjs.h"
#include "PluggableUSBMSD.h"
#include "QSPIFBlockDevice.h"

static QSPIFBlockDevice bd(PD_11, PD_12, PF_7, PD_13,  PF_10, PG_6, QSPIF_POLARITY_MODE_1, 40000000);

void USBMSD::begin()
{
  int err = getFileSystem().mount(&bd);
  if (err) {
    err = getFileSystem().reformat(&bd);
  }
}

mbed::FATFileSystem& USBMSD::getFileSystem()
{
  static mbed::FATFileSystem fs("fs");
  return fs;
}

USBMSD MassStorage(&bd);

extern "C" int qjs_main(int argc, char **argv);

char* args[] = { "-h", "/fs/main.js" };

bool hasMainJs() {
  DIR *dir;
  struct dirent *ent;
  bool found = false;
  if ((dir = opendir("/fs")) != NULL) {
    /* print all the files and directories within directory */
    while ((ent = readdir(dir)) != NULL) {
      if (String (ent->d_name) == "main.js") {
        found = true;
      }
    }
    closedir(dir);
  }
  return found;
}

REDIRECT_STDOUT_TO(Serial);

void setup() {
  MassStorage.begin();
  Serial.begin(115200);

  while (!hasMainJs()) {
    delay(1000);
  }

  while (!Serial);

  printf("starting qjs_main\n");

  qjs_main(2, args);

  printf("qjs_main done\n");
}

void loop() {
  // put your main code here, to run repeatedly:

}
