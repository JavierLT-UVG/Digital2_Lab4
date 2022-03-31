#include <SPI.h>
#include <SD.h>

#define SlaveSelect PA_3

File root;
File myFile;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  SPI.setModule(0);

  // Rutina que despliega todos los archivos en la SD
  showFiles();

  // Esperar input del usuario
  Serial.println("Ingrese el nombre del archivo que quiere abrir");
  while (Serial.available() == 0) {
  }
  // Almacenar en variable el string del usuario
  String nombreArchivo = Serial.readString();
  // Convertir el String a concatenación de chars
  char nombreArchivo_char[15];
  nombreArchivo.toCharArray(nombreArchivo_char,15);
  // Borrar el último char de la cadena (salto de línea)
  int length = strlen(nombreArchivo_char);
  nombreArchivo_char[length-1] = '\0';
  
  // Leer el archivo con el nombre que proporcionó el usuario
  readFile(nombreArchivo_char);
}

void loop()
{
  // nothing happens after setup finishes.
}

void showFiles() {
  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
  pinMode(SlaveSelect, OUTPUT);

  if (!SD.begin(SlaveSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  root = SD.open("/");

  printDirectory(root, 0);

  Serial.println("done!");
}


void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}


void readFile(char nombre[]) {
  // re-open the file for reading:
  myFile = SD.open(nombre);
  if (myFile) {
    Serial.print(nombre);
    Serial.println(":");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening file");
  }
}
