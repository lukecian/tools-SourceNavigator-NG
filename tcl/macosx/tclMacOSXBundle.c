/*
 * tclMacOSXBundle.c --
 *
 *	This file implements functions that inspect CFBundle structures
 *      on MacOS X.
 *
 *      Copyright 2001, Apple Computer, Inc.
 *
 *      The following terms apply to all files originating from Apple
 *      Computer, Inc. ("Apple") and associated with the software
 *      unless explicitly disclaimed in individual files.
 *
 *
 *      Apple hereby grants permission to use, copy, modify,
 *      distribute, and license this software and its documentation
 *      for any purpose, provided that existing copyright notices are
 *      retained in all copies and that this notice is included
 *      verbatim in any distributions. No written agreement, license,
 *      or royalty fee is required for any of the authorized
 *      uses. Modifications to this software may be copyrighted by
 *      their authors and need not follow the licensing terms
 *      described here, provided that the new terms are clearly
 *      indicated on the first page of each file where they apply.
 *
 *
 *      IN NO EVENT SHALL APPLE, THE AUTHORS OR DISTRIBUTORS OF THE
 *      SOFTWARE BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL,
 *      INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF
 *      THIS SOFTWARE, ITS DOCUMENTATION, OR ANY DERIVATIVES THEREOF,
 *      EVEN IF APPLE OR THE AUTHORS HAVE BEEN ADVISED OF THE
 *      POSSIBILITY OF SUCH DAMAGE.  APPLE, THE AUTHORS AND
 *      DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES, INCLUDING,
 *      BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS
 *      SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, AND APPLE,THE
 *      AUTHORS AND DISTRIBUTORS HAVE NO OBLIGATION TO PROVIDE
 *      MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 *      GOVERNMENT USE: If you are acquiring this software on behalf
 *      of the U.S. government, the Government shall have only
 *      "Restricted Rights" in the software and related documentation
 *      as defined in the Federal Acquisition Regulations (FARs) in
 *      Clause 52.227.19 (c) (2).  If you are acquiring the software
 *      on behalf of the Department of Defense, the software shall be
 *      classified as "Commercial Computer Software" and the
 *      Government shall have only "Restricted Rights" as defined in
 *      Clause 252.227-7013 (c) (1) of DFARs.  Notwithstanding the
 *      foregoing, the authors grant the U.S. Government and others
 *      acting in its behalf permission to use and distribute the
 *      software in accordance with the terms specified in this
 *      license.
 */
 
 #include <CoreFoundation/CoreFoundation.h>
 #include "tcl.h"

/*
 *----------------------------------------------------------------------
 *
 * Tcl_MacOSXOpenBundleResources --
 *
 *	Given the bundle name for a shared library, this routine
 *	sets libraryPath to the Resources/Scripts directory 
 *	in the framework package.  If hasResourceFile is
 *	true, it will also open the main resource file for the bundle.
 *
 *
 * Results:
 *	TCL_OK if the bundle could be opened, and the Scripts folder found.
 *      TCL_ERROR otherwise.
 *
 * Side effects:
 *	libraryVariableName may be set, and the resource file opened.
 *
 *----------------------------------------------------------------------
 */

int
Tcl_MacOSXOpenBundleResources(Tcl_Interp *interp,
        CONST char *bundleName,
        int hasResourceFile,       
        int maxPathLen,
        char *libraryPath)
{
    CFBundleRef bundleRef;
    CFStringRef bundleNameRef;
    
    libraryPath[0] = '\0';
    
    bundleNameRef = CFStringCreateWithCString(NULL, 
            bundleName, kCFStringEncodingUTF8);
            
    bundleRef = CFBundleGetBundleWithIdentifier(bundleNameRef);
    CFRelease(bundleNameRef);
    
    if (bundleRef == 0) {
        return TCL_ERROR;
    } else {
        CFURLRef libURL;
        
        if (hasResourceFile) {
            short refNum;
            refNum = CFBundleOpenBundleResourceMap(bundleRef);
        }
                
        libURL = CFBundleCopyResourceURL(bundleRef, 
	            CFSTR("Scripts"), 
		    NULL, 
		    NULL);

        if (libURL != NULL) {
            /* 
             * FIXME: This is a quick fix, it is probably not right 
             * for internationalization. 
             */
            
            if (CFURLGetFileSystemRepresentation (libURL, true,
                    libraryPath, maxPathLen)) {
            }
            CFRelease(libURL);
        } else {
            return TCL_ERROR;
        }
    }
    
    return TCL_OK;
}

