//
//  localConfigsTemplate.h
//  TMG Shape Displays
//
//  Created by Daniel Windham on 06/25/2015.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

// Local configuration setup instructions:
//
// 1. Create the file `src/localConfigs.h` _WITHOUT_ using Xcode [*]
// 2. Copy the template file below into localConfigs.h
// 3. Uncomment the definition that matches the machine you're using
//
// Note that this project's .gitignore file tells git to ignore any file called
// `localConfigs.h`, so your local configurations will remain local to you.
//
// [*] If you use Xcode's "New File..." feature in this step, you will create
//     unnecessary new Xcode project metadata, and if you commit these changes
//     it will create ugly code history.
//
// This seems to be the best solution for local configuration setup. Other
// strategies run into trouble with git hard-resets or require build hooks or
// other external scripts. See http://stackoverflow.com/questions/4348590/ .


// Local Configs Template File
// ---------------------------

#pragma once


// Uncomment the appropriate line to specify which shape display you are using.

//#define LOCAL_CONFIGS__USE_INFORM
//#define LOCAL_CONFIGS__USE_TRANSFORM
//#define LOCAL_CONFIGS__USE_COOPERFORM