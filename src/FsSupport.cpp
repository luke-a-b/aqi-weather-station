/*====================================================================================
  This sketch contains support functions for the ESP6266 FS filing system

  Created by Bodmer 15th Jan 2017
  ==================================================================================*/
#include "FsSupport.h"

//====================================================================================
//                 Print a LittleFS directory list
//====================================================================================

uint32_t FsSupport::listFiles(String dirName, uint8_t depth)
{
    yield();
    fs::Dir dir = LittleFS.openDir(dirName); // Root directory
    uint32_t totalBytes = 0;
    String line = "====================================================";

    if (depth == 0)
    {
        Serial.println();
        Serial.println("Listing all files found in '" + dirName + "'");

        Serial.println(line);
        Serial.println("  File name               Size");
        Serial.println(line);
    }
    while (dir.next())
    {
        String fileName = dir.fileName();
        if (dir.isDirectory())
        {
            totalBytes += listFiles(dirName + fileName + "/", depth + 1);
        }
        else
        {
            Serial.print(fileName);
            int spaces = 25 - fileName.length(); // Tabulate nicely
            while (spaces--)
                Serial.print(" ");
            fs::File f = dir.openFile("r");
            Serial.print(f.size());
            Serial.println(" bytes");
            totalBytes += f.size();
        }
        yield();
    }
    if (depth == 0)
    {
        Serial.println();
        Serial.print("Total = ");
        int spaces = 25 - 8; // Tabulate nicely
        while (spaces--)
            Serial.print(" ");
        Serial.print(totalBytes);
        Serial.println(" bytes");

        Serial.println(line);
        Serial.println();
    }
    yield();
}
//====================================================================================
