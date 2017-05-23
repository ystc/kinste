/*******************************************************************************
* Filename: main.cpp
* Author: ystc
* Created: 12.01.2016
* Last change: 27.01.2017
*******************************************************************************/

/*******************************************************************************
 *
 * Copyright (C) YLOG Industry Solutions GmbH
 *
 * The copyright to the computer program(s) herein is the property
 * of YLOG Industry Solutions GmbH. The program(s) may be used and/or copied
 * only with the written permission of YLOG or in accordance with the
 * terms and conditions stipulated in the agreement/contract under
 * which the program(s) have been supplied.
 *
 ******************************************************************************/

#include <iostream>
#include "inc/MainPageControl.h"
#include "inc/UserInterface.h"

//------------------------------------------------------------------------------
void printHelp()
{
  cout << "Shuttle test Environment (STE) Version 1.3.0" << endl;
  cout << "Using Qt version 5.5.1" << endl << endl;
  cout << "Usage:\t\t./ste\n" << endl;
  cout << "Arguments:" << endl;
  cout << "\t\t-v\t\t\n\t\t-i [shuttle-ip]\n";
  cout << "\t\t-f [tasi-run-file]\n";
}

//------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
  if(argc > 3) {
    printHelp();
    return 0;
  }
  else if(argc == 2){
    if(strcmp(argv[1], "-v") == 0){
      printHelp();
      return 0;
    }
    else{
      printHelp();
    }
  }
  else {
    MainPageControl main_page;
    main_page.show(argc, argv);
  }
}

