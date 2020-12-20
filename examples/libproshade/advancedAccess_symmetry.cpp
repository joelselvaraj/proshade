/*! \file advancedAccess_reboxing.cpp
    \brief This file showcases how the ProSHADE symetry detection can be computed using the dynamic library's advanced interface.

    This code is an example of how any C++ project linking the ProSHADE library can access the advanced ProSHADE interface. More specifically, in this example, the structure for which symmetry is to be detected is read in, processed, mapped onto spheres and its spherical harmonics decomposition is computed. Next, the
    structure has its rotation function computed and the symmetry is detected from peaks in the self-rotation function next. ProSHADE allows accessing the recommended symmetry type and fold values using the functions shown in the code. Should the user be interested in specifying the symmetry for ProSHADE to find, it can
    be done as shown in the second part of the code.

    Copyright by Michal Tykac and individual contributors. All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
    1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    3) Neither the name of Michal Tykac nor the names of this code's contributors may be used to endorse or promote products derived from this software without specific prior written permission.

    This software is provided by the copyright holder and contributors "as is" and any express or implied warranties, including, but not limitted to, the implied warranties of merchantibility and fitness for a particular purpose are disclaimed. In no event shall the copyright owner or the contributors be liable for any direct, indirect, incidental, special, exemplary, or consequential damages (including, but not limitted to, procurement of substitute goods or services, loss of use, data or profits, or business interuption) however caused and on any theory of liability, whether in contract, strict liability or tort (including negligence or otherwise) arising in any way out of the use of this software, even if advised of the possibility of such damage.

    \author    Michal Tykac
    \author    Garib N. Murshudov
    \version   0.7.5.0
    \date      DEC 2020
*/

//==================================================== ProSHADE
#include "../../src/proshade/ProSHADE.hpp"

//==================================================== Main
int main ( int argc, char **argv )
{
    //================================================ Create the settings object and parse the command line arguments
    ProSHADE_Task task                                = Symmetry;                            // Setting the task ahead sets most of the default settings to best values for the task.
    ProSHADE_settings* settings                       = new ProSHADE_settings ( task );      // Creating the ProSHADE_settings object, which caries all of the settings and where everything can be set.

    //================================================ Required settings
    
    
    //================================================ Further useful settings
    settings->setSymmetryRotFunPeaks                  ( true );                              // Should the new angle-axis space symmetry detection be used?
    settings->setBicubicInterpolationSearch           ( true );                              // Should bi-cubic interpolation between peak grid indices be done?
    settings->setMaxSymmetryFold                      ( 30 );                                // The maximum prime number fold that will be searched for.
    settings->forceP1                                 = true;                                // Should PDB files be forced to have P1 spacegroup?
    settings->removeWaters                            = true;                                // Should PDB files have their water molecules removed?
    settings->firstModelOnly                          = true;                                // Should PDB files have only their first model used, or should ProSHADE use all models?
    settings->setProgressiveSphereMapping             ( true );                              // Should smaller spheres be less sampled? It is considerably faster, but may sacrifice some (little) accuracy.
    settings->setMapResolutionChange                  ( true );                              // Should maps be re-sample to the computation resolution using reciprocal-space re-sampling?
    settings->setMapResolutionChangeTriLinear         ( false );                             // Should maps be re-sample to the computation resolution using real-space tri-linear interpolation?
    settings->setPeakNeighboursNumber                 ( 1 );                                 // Numer of points in each direction which needs to be lower in order for the central point to be considered a peak.
    settings->setPeakNaiveNoIQR                       ( -999.9 );                            // Peak searching threshold for too low peaks in number of inter-quartile ranges from median of the non-peak point values.
    settings->setMissingPeakThreshold                 ( 0.3 );                               // Fraction of peaks that can be missing for missing axis search to be initiated.
    settings->setAxisComparisonThreshold              ( 0.1 );                               // The dot product difference within which two axes are considered the same.
    settings->setMinimumPeakForAxis                   ( 0.3 );                               // The minimum peak height for axis to be used.
//    settings->setRequestedSymmetry                    ( "C" );                               // Which symmetry type (C,D,T,O or I) is requested to be detected? If none, then leave empty
//    settings->setRequestedFold                        ( 6 );                                 // For C and D symmetries, which symmetry fold is requested to be detected? If none, leave 0.
    settings->setMapCentering                         ( true );                              // Move structure COM to the centre of map box?
    settings->setExtraSpace                           ( 10.0 );                              // Extra space in Angs to be added when creating internap map representation. This helps avoid map effects from other cells.
    settings->setResolution                           ( 6.0 );                              // The resolution to which the calculations will be done. NOTE: Not necessarily the resolution of the structure!
    settings->verbose                                 = -1;                                  // How verbose should the run be? -1 Means no verbal output at all.
    
    //================================================ All other (possibly other tasks related) settings
    settings->setMapInversion                         ( false );                             // Should all map positions x,y,z be swapped to -x,-y,-z? Use this only if your helices have the wrong hand as a result of first runs of map computation.
    settings->setBandwidth                            ( 0 );                                 // The spherical harmonics bandwidth to which to compute. Set to 0 for automatic determination.
    settings->setSphereDistances                      ( 0.0 );                               // The distance between spheres. Use 0.0 for automatic determination.
    settings->setIntegrationOrder                     ( 0 );                                 // The order of the Gauss-Legendre integration computation. Set to 0 for automatic determination.
    settings->setTaylorSeriesCap                      ( 10 );                                // Set the Taylor series approximation cap. 10 seems like a fast and accurate value, but feel free to change.
    settings->setEnergyLevelsComputation              ( true );                              // Should energy levels descriptor be computed, assuming Distances are required (irrelevant otherwise)?
    settings->setTraceSigmaComputation                ( true );                              // Should trace sigma descriptor be computed, assuming Distances are required (irrelevant otherwise)?
    settings->setRotationFunctionComputation          ( true );                              // Should rotation function descriptor be computed, assuming Distances are required (irrelevant otherwise)?
    settings->setEnLevShellWeight                     ( 1.0 );                               // The weighting of shell distances for energy levels descriptor.
    settings->setPDBBFactor                           ( -1.0 );                              // Should all B-factors in a PDB file changed to this value? If no, set to negative value.
    settings->setPhaseUsage                           ( true );                              // Use full maps, or Patterson-like maps?
    settings->setOverlaySaveFile                      ( "overlayResuls" );                   // Filename where the overlayed moving structure should be saved.
    settings->setOverlayJsonFile                      ( "movedStructureOperations.json" );   // Filename where the overlay operations should be saved.
    settings->setNormalisation                        ( false );                             // Should internal map representation be normalised to mean 0 and standard deviation 1?
    settings->setMapReboxing                          ( false );                             // Should the structure be re-boxed? Required masking to be done in order to be meaningful.
    settings->setOutputFilename                       ( "reBoxed" );                         // Filename to where re-boxed structure will be written to.
    settings->setBoundsSpace                          ( 3.0 );                               // The extra space in Angs to add to the minimal boundaries when re-boxing.
    settings->setBoundsThreshold                      ( 0 );                                 // If two boundaries are within this threshold, the smaller one will be increased to have the same value as the larger one.
    settings->setSameBoundaries                       ( false );                             // Make multiple structures have the same boundaries. This is useful for half-maps.
    settings->setMasking                              ( false );                             // Should maps be masked by blurring?
    settings->setMaskBlurFactor                       ( 350.0 );                             // If masking, what blur factor should be used? 350 seems to work for most maps.
    settings->setMaskIQR                              ( 3.0 );                               // Number of inter-quartile ranges from median to use as the masking threshold.
    settings->setMaskSaving                           ( false );                             // Should map mask be saved?
    settings->setMaskFilename                         ( "maskFile" );                        // The filename (no extension) to which the map masks will be saved into.

    //================================================ Print all the settings values
//    settings->printSettings                           ( );                                   // Prints all the ProSHADE_settings values. Mostly for debugging purposes.

    //================================================ Create the structure objects
    ProSHADE_internal_data::ProSHADE_data* simpleSym  = new ProSHADE_internal_data::ProSHADE_data ( settings ); // This line initialises the structure object
    
    //================================================ Read in the structures
    simpleSym->readInStructure                        ( "./emd_6324.map", 0, settings );   // This is how a particular structure file is read into the ProSHADE object. This example uses EMD 6324 (PDB 3JA7)

    //================================================ Process internal map
    simpleSym->processInternalMap                     ( settings ); // This function does the internal map processing such as map centering, masking, invertion, phase removal, etc. for the structure which calls it.
    
    //================================================ Map to spheres
    simpleSym->mapToSpheres                           ( settings ); // This function maps the processed internal map onto a set of concentric spheres in preparation for spherical harmonics computation for the structure which calls it.
    
    //================================================ Compute spherical harmonics decompostion
    simpleSym->computeSphericalHarmonics              ( settings ); // This function computes the spherical harmonics for this structure.
    
    //================================================ Compute self-rotation function
    simpleSym->getRotationFunction                    ( settings ); // This function computes the self-rotation function for the structure calling it.
    
    //================================================ Detect the recommended symmetry
    std::vector< proshade_double* > recomSymAxes;
    std::vector< std::vector< proshade_double > > allCSymAxes;
    simpleSym->detectSymmetryInStructure              ( settings, &recomSymAxes, &allCSymAxes ); // This function does the symmetry detection in the peaks of the self-rotation function. Once complete, the results can be accessed as shown below
    std::string symmetryType                          = simpleSym->getRecommendedSymmetryType ( settings ); // This is how the recommended symmetry type can be obtained.
    proshade_unsign symmetryFold                      = simpleSym->getRecommendedSymmetryFold ( settings ); // This is how the recommended symmetry fold can be obtained.
    
    //================================================ Write out the symmetry detection results
    std::cout << "Detected symmetry: " << symmetryType << "-" << symmetryFold << " with axes:" << std::endl;
    for ( proshade_unsign axIt = 0; axIt < static_cast<proshade_unsign> ( recomSymAxes.size() ); axIt++ )
    {
        std::cout << "Symmetry axis number " << axIt << ": Fold " << recomSymAxes.at(axIt)[0] << " XYZ: " << recomSymAxes.at(axIt)[1] << " ; " << recomSymAxes.at(axIt)[2] << " ; " << recomSymAxes.at(axIt)[3] << " Angle (radians): " << recomSymAxes.at(axIt)[4] << " and axis peak: " << recomSymAxes.at(axIt)[5] << std::endl;
    }
    
    //================================================ Expected output
//  Detected symmetry: D-12 with axes:
//  Symmetry axis number 0: Fold 12 XYZ: 0.000565414 ; 1.87314e-05 ; 0.999986 Angle (radians): 0.523599 and axis peak: 0.956675
//  Symmetry axis number 1: Fold 2 XYZ: 0.971188 ; 0.205281 ; -0.00380724 Angle (radians): 3.14159 and axis peak: 0.471463
    
    //================================================ Find all C axes
    std::vector < std::vector< proshade_double > > allCs = settings->allDetectedCAxes;
    std::cout << "Found total of " << allCs.size() << " cyclic symmetry axes." << std::endl;
    
    //================================================ Expected output
//  Found total of 13 cyclic symmetry axes.
    
    
//  NOTE: To get all the point group elements, one needs to supply the list of all cyclic point groups which comprise the
//        requested point group. This is relatively simple for T, O and I symmetries, as such list is already produced by
//        ProSHADE - see the following examples:
//
//        std::vector<std::vector< proshade_double > > groupElements = symmetryStructure->getAllGroupElements ( settings, settings->allDetectedTAxes, "T" );
//        std::vector<std::vector< proshade_double > > groupElements = symmetryStructure->getAllGroupElements ( settings, settings->allDetectedOAxes, "O" );
//        std::vector<std::vector< proshade_double > > groupElements = symmetryStructure->getAllGroupElements ( settings, settings->allDetectedIAxes, "I" );
//
//        For C point groups, this is also simple, as one can select the required >index< from the allCs variable and use
//
//        std::vector< proshade_unsign > bestCAxesList;
//        bestCAxesList.emplace_back ( index );
//        std::vector<std::vector< proshade_double > > groupElements = symmetryStructure->getAllGroupElements ( settings, bestCAxesList, "C" );
//
//        The only problem comes when D is to be used, as ProSHADE gives a vector of all combinations (also as vector) of cyclic point groups which form
//        D point groups. Therefore, to select the recommended D point group from this list, a search needs to be done. This is shown in the following code.
    
    //================================================ Find which D axes combination was reported as best
    std::vector< proshade_unsign > bestDAxesList;
    bool firstMatch = false; bool secondMatch = false;
    for ( int dIt = 0; dIt < static_cast<int> ( settings->allDetectedDAxes.size() ); dIt++ )
    {
        firstMatch                                    = false;
        secondMatch                                   = false;
        
        for ( proshade_unsign recIt = 0; recIt < static_cast<proshade_unsign> ( recomSymAxes.size() ); recIt++ )
        {
            if ( ( allCs.at(settings->allDetectedDAxes.at(dIt).at(0))[1] == recomSymAxes.at(recIt)[1] ) &&
                 ( allCs.at(settings->allDetectedDAxes.at(dIt).at(0))[2] == recomSymAxes.at(recIt)[2] ) &&
                 ( allCs.at(settings->allDetectedDAxes.at(dIt).at(0))[3] == recomSymAxes.at(recIt)[3] ) )
            {
                firstMatch                            = true;
            }
        }
        
        for ( proshade_unsign recIt = 0; recIt < static_cast<proshade_unsign> ( recomSymAxes.size() ); recIt++ )
        {
            if ( ( allCs.at(settings->allDetectedDAxes.at(dIt).at(1))[1] == recomSymAxes.at(recIt)[1] ) &&
                 ( allCs.at(settings->allDetectedDAxes.at(dIt).at(1))[2] == recomSymAxes.at(recIt)[2] ) &&
                 ( allCs.at(settings->allDetectedDAxes.at(dIt).at(1))[3] == recomSymAxes.at(recIt)[3] ) )
            {
                secondMatch                           = true;
            }
        }
        
        if ( firstMatch && secondMatch )
        {
            bestDAxesList.emplace_back                ( settings->allDetectedDAxes.at(dIt).at(0) );
            bestDAxesList.emplace_back                ( settings->allDetectedDAxes.at(dIt).at(1) );
        }
    }
    
    //================================================ Get point group elements for the best D point group
    std::vector<std::vector< proshade_double > > groupElements = simpleSym->getAllGroupElements ( settings, bestDAxesList, "D" );
    
    //================================================ Print results
    std::cout << "Point group D" << allCs.at(bestDAxesList.at(0))[0] << "-" << allCs.at(bestDAxesList.at(1))[0] << " has been found to have " << groupElements.size() << " group elements, with the first element (excluding the identity one) having rotation matrix:" << std::fixed << std::setprecision(2) << std::showpos << std::endl;
    std::cout << groupElements.at(1).at(0) << " | " << groupElements.at(1).at(1) << " | " << groupElements.at(1).at(2) << std::endl;
    std::cout << groupElements.at(1).at(3) << " | " << groupElements.at(1).at(4) << " | " << groupElements.at(1).at(5) << std::endl;
    std::cout << groupElements.at(1).at(6) << " | " << groupElements.at(1).at(7) << " | " << groupElements.at(1).at(8) << std::endl << std::endl;
    
    //================================================ Expected output
//  Point group D12-2 has been found to have 24 group elements, with the first element (excluding the identity one) having rotation matrix:
//  +0.87 | -0.50 | +0.00
//  +0.50 | +0.87 | -0.00
//  +0.00 | +0.00 | +1.00
    
    //================================================ Release the object
    delete simpleSym;
    
    //================================================ Now, detect the symmetry again, but this time with user defined requested symmetry
    settings->setRequestedSymmetry                    ( "C" );                               // Which symmetry type (C,D,T,O or I) is requested to be detected? If none, then leave empty
    settings->setRequestedFold                        ( 4 );                                 // For C and D symmetries, which symmetry fold is requested to be detected? If none, leave 0.
    ProSHADE_internal_data::ProSHADE_data* requestSym = new ProSHADE_internal_data::ProSHADE_data ( settings ); // This line initialises the structure object
    requestSym->readInStructure                       ( "/Users/mysak/LMB/proshade/exp/demo/testMap2.map", 0, settings );   // This is how a particular structure file is read into the ProSHADE object.
    requestSym->processInternalMap                    ( settings );
    requestSym->mapToSpheres                          ( settings );
    requestSym->computeSphericalHarmonics             ( settings );
    requestSym->getRotationFunction                   ( settings );
    
    //================================================ Detect the recommended symmetry
    std::vector< proshade_double* > reqSymAxes;
    allCSymAxes.clear();
    requestSym->detectSymmetryInStructure             ( settings, &reqSymAxes, &allCSymAxes );
    symmetryType                                      = requestSym->getRecommendedSymmetryType ( settings );
    symmetryFold                                      = requestSym->getRecommendedSymmetryFold ( settings );
    
    //================================================ Report the results for the requested symmetry
    if ( ( symmetryType == settings->requestedSymmetryType ) && ( symmetryFold == settings->requestedSymmetryFold ) )
    {
        std::cout << "Detected symmetry: " << symmetryType << "-" << symmetryFold << " as requested. The axes are:" << std::endl;
        for ( proshade_unsign axIt = 0; axIt < static_cast<proshade_unsign> ( reqSymAxes.size() ); axIt++ )
        {
            std::cout << "Symmetry axis number " << axIt << ": Fold " << reqSymAxes.at(axIt)[0] << " XYZ: " << reqSymAxes.at(axIt)[1] << " ; " << reqSymAxes.at(axIt)[2] << " ; " << reqSymAxes.at(axIt)[3] << " Angle (radians): " << reqSymAxes.at(axIt)[4] << " and axis peak: " << reqSymAxes.at(axIt)[5] << std::endl;
        }
    }
    else
    {
        std::cout << "!!! Warning !!! ProSHADE failed to detect the requested " << settings->requestedSymmetryType << "-" << settings->requestedSymmetryFold << " symmetry. If you believe the symmetry should be there, you may want to try to set the map centering to true, decrease the resolution to reduce the effect of surface details or play around with the missing peak and axis comparison thresholds." << std::endl;
    }
    
    //================================================ Expected output
//  Detected symmetry: C-4 as requested. The axes are:
//  Symmetry axis number 0: Fold +4.00 XYZ: +0.00 ; -0.00 ; +1.00 Angle (radians): +1.57 and axis peak: +0.98
 
    //================================================ Release the settings and runProshade objects
    delete requestSym;
    delete settings;
    
    //================================================ DONE
    return EXIT_SUCCESS;
}
