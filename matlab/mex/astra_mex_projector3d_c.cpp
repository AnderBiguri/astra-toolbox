/*
-----------------------------------------------------------------------
Copyright: 2010-2015, iMinds-Vision Lab, University of Antwerp
           2014-2015, CWI, Amsterdam

Contact: astra@uantwerpen.be
Website: http://sf.net/projects/astra-toolbox

This file is part of the ASTRA Toolbox.


The ASTRA Toolbox is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

The ASTRA Toolbox is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with the ASTRA Toolbox. If not, see <http://www.gnu.org/licenses/>.

-----------------------------------------------------------------------
$Id$
*/

/** \file astra_mex_projector3d_c.cpp
 *
 *  \brief Create and manage 3d projectors in the ASTRA workspace
 */

#include <mex.h>
#include "mexHelpFunctions.h"
#include "mexInitFunctions.h"

#include "astra/Globals.h"

#include "astra/Projector3D.h"
#include "astra/AstraObjectManager.h"
#include "astra/AstraObjectFactory.h"

#include "astra/ProjectionGeometry3D.h"
#include "astra/VolumeGeometry3D.h"

#include <map>
#include <vector>

using namespace std;
using namespace astra;

//-----------------------------------------------------------------------------------------
/**
* [pid] = astra_mex_projector('create', cfgstruct);
*/
void astra_mex_projector3d_create(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{ 
	if (nrhs < 2) {
		mexErrMsgTxt("Not enough arguments.  See the help document for a detailed argument list. \n");
		return;
	}

	if (!mxIsStruct(prhs[1])) {
		mexErrMsgTxt("Argument 1 not a valid MATLAB struct. \n");
	}

	// turn MATLAB struct to an XML-based Config object
	Config* cfg = structToConfig("Projector3D", prhs[1]);

	// create algorithm
	CProjector3D* pProj = CProjector3DFactory::getSingleton().create(*cfg);
	if (pProj == NULL) {
		delete cfg;
		mexErrMsgTxt("Error creating Projector3D. \n");
		return;
	}
	delete cfg;

	// store projector
	int iIndex = CProjector3DManager::getSingleton().store(pProj);

	// step4: set output
	if (1 <= nlhs) {
		plhs[0] = mxCreateDoubleScalar(iIndex);
	}

	}

//-----------------------------------------------------------------------------------------
/**
* astra_mex_projector3d('destroy', pid1, pid2, ...);
*/
void astra_mex_projector3d_destroy(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{
	// step1: read input
	if (nrhs < 2) {
		mexErrMsgTxt("Not enough arguments.  See the help document for a detailed argument list. \n");
		return;
	}

	for (int i = 1; i < nrhs; i++) {
		int iPid = (int)(mxGetScalar(prhs[i]));
		CProjector3DManager::getSingleton().remove(iPid);
	}
}

//-----------------------------------------------------------------------------------------
/**
 * astra_mex_projector3d('clear');
 */
void astra_mex_projector3d_clear(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{
	CProjector3DManager::getSingleton().clear();
}


//-----------------------------------------------------------------------------------------
/**
* [proj_geom] = astra_mex_projector3d('get_projection_geometry', pid);
*/
void astra_mex_projector3d_get_projection_geometry(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{
	// step1: read input
	if (nrhs < 2) {
		mexErrMsgTxt("Not enough arguments.  See the help document for a detailed argument list. \n");
		return;
	}
	int iPid = (int)(mxGetScalar(prhs[1]));

	// step2: get projector
	CProjector3D* pProjector;
	if (!(pProjector = CProjector3DManager::getSingleton().get(iPid))) {
		mexErrMsgTxt("Projector not found.\n");
		return;
	}

	// step3: get projection_geometry and turn it into a MATLAB struct
	if (1 <= nlhs) {
		plhs[0] = configToStruct(pProjector->getProjectionGeometry()->getConfiguration());
	}
}

//-----------------------------------------------------------------------------------------
/**
* [recon_geom] = astra_mex_projector3d('get_volume_geometry', pid);
*/
void astra_mex_projector3d_get_volume_geometry(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{
	// step1: read input
	if (nrhs < 2) {
		mexErrMsgTxt("Not enough arguments.  See the help document for a detailed argument list. \n");
		return;
	}
	int iPid = (int)(mxGetScalar(prhs[1]));

	// step2: get projector
	CProjector3D* pProjector;
	if (!(pProjector = CProjector3DManager::getSingleton().get(iPid))) {
		mexErrMsgTxt("Projector not found.\n");
		return;
	}

	// step3: get projection_geometry and turn it into a MATLAB struct
	if (1 <= nlhs) {
		plhs[0] = configToStruct(pProjector->getVolumeGeometry()->getConfiguration());
	}
}

//-----------------------------------------------------------------------------------------
/**
* [weights] = astra_mex_projector3d('weights_single_ray', pid, projection_index, detector_index);
*/
void astra_mex_projector_weights_single_ray(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{ 
	//// step1: get input
	//if (nrhs < 4) {
	//	mexErrMsgTxt("Not enough arguments.  See the help document for a detailed argument list. \n");
	//	return;
	//}
	//int iPid = (int)(mxGetScalar(prhs[1]));
	//int iProjectionIndex = (int)(mxGetScalar(prhs[2]));
	//int iDetectorIndex = (int)(mxGetScalar(prhs[3]));

	//// step2: get projector
	//CProjector3D* pProjector;
	//if (!(pProjector = CProjector3DManager::getSingleton().get(iPid))) {
	//	mexErrMsgTxt("Projector not found.\n");
	//	return;
	//}
	//
	//// step3: create output vars
	//int iStoredPixelCount;
	//int iMaxPixelCount = pProjector->getProjectionWeightsCount(iProjectionIndex);
	//SWeightedPixel* pPixelsWeights = new SWeightedPixel3D[iMaxPixelCount];
	//
	//// step4: perform operation
	//pProjector->computeSingleRayWeights(iProjectionIndex, 
	//									iDetectorIndex, 
	//									pPixelsWeights, 
	//									iMaxPixelCount, 
	//									iStoredPixelCount);

	//// step5: return output
	//if (1 <= nlhs) {
	//	mwSize dims[2];
	//	dims[0] = iStoredPixelCount;
	//	dims[1] = 2;

	//	plhs[0] = mxCreateNumericArray(2, dims, mxDOUBLE_CLASS, mxREAL);
	//	double* out = mxGetPr(plhs[0]);

	//	for (int col = 0; col < iStoredPixelCount; col++) {
	//		out[col] = pPixelsWeights[col].m_iIndex;
	//		out[iStoredPixelCount+col] = pPixelsWeights[col].m_fWeight;
	//		//cout << pPixelsWeights[col].m_iIndex << " " << pPixelsWeights[col].m_fWeight <<endl;
	//	}	
	//}
	//
	//// garbage collection
	//delete[] pPixelsWeights;
}

////-----------------------------------------------------------------------------------------
///**
//* [weights] = astra_mex_projector('weights_projection', pid, projection_index);
//*/
//void astra_mex_projector_weights_projection(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
//{ 
//	// step1: get input
//	if (nrhs < 3) {
//		mexErrMsgTxt("Not enough arguments.  See the help document for a detailed argument list. \n");
//		return;
//	}
//	int iPid = (int)(mxGetScalar(prhs[1]));
//	int iProjectionIndex = (int)(mxGetScalar(prhs[2]));
//
//	// step2: get projector
//	CProjector2D* pProjector;
//	if (!(pProjector = CProjectorManager::getSingleton().get(iPid))) {
//		mexErrMsgTxt("Projector not found.\n");
//		return;
//	}
//
//	// step3: create output vars
//	SWeightedPixel2D* pPixelsWheights = new SWeightedPixel2D[pProjector->getProjectionWeightsCount(iProjectionIndex)];
//	int* piRayStoredPixelCount = new int[pProjector->getProjectionGeometry()->getDetectorCount()];
//
//	// step4: perform operation
//	pProjector->computeProjectionRayWeights(iProjectionIndex, pPixelsWheights, piRayStoredPixelCount);
//
//	// step5: return output
//	if (1 <= nlhs) {
//		// get basic values
//		int iMatrixSize = pProjector->getVolumeGeometry()->getWindowLengthX() *
//						  pProjector->getVolumeGeometry()->getWindowLengthY();
//		int iDetectorCount = pProjector->getProjectionGeometry()->getDetectorCount();
//		int iTotalStoredPixelCount = 0;
//		for (int i = 0; i < iDetectorCount; i++) {
//			iTotalStoredPixelCount += piRayStoredPixelCount[i];
//		}
//
//		// create matlab sparse matrix
//		plhs[0] = mxCreateSparse(iMatrixSize,				// number of rows (#pixels)
//							     iDetectorCount,			// number of columns (#detectors)
//								 iTotalStoredPixelCount,	// number of non-zero elements
//								 mxREAL);					// element type
//		double* values = mxGetPr(plhs[0]);
//		mwIndex* rows = mxGetIr(plhs[0]);
//		mwIndex* cols = mxGetJc(plhs[0]);
//		
//		int currentBase = 0;
//		int currentIndex = 0;
//		for (int i = 0; i < iDetectorCount; i++) {
//			for (int j = 0; j < piRayStoredPixelCount[i]; j++) {
//				values[currentIndex + j] = pPixelsWheights[currentBase + j].m_fWeight;
//				rows[currentIndex + j] = pPixelsWheights[currentBase + j].m_iIndex;
//			}
//					
//			currentBase += pProjector->getProjectionWeightsCount(iProjectionIndex) / pProjector->getProjectionGeometry()->getDetectorCount();
//			currentIndex += piRayStoredPixelCount[i];
//		}
//		cols[0] = piRayStoredPixelCount[0];
//		for (int j = 1; j < iDetectorCount; j++) {
//			cols[j] = cols[j-1] + piRayStoredPixelCount[j];
//		}
//		cols[iDetectorCount] = iTotalStoredPixelCount;
//	}
//	
//}
//
////-----------------------------------------------------------------------------------------
///**
//* output = astra_mex_projector('splat', pid, x, y);
//*/
//void astra_mex_projector_splat(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
//{ 
//	// step1: get input
//	if (nrhs < 4) {
//		mexErrMsgTxt("Not enough arguments.  See the help document for a detailed argument list. \n");
//		return;
//	}
//	int iPid = (int)(mxGetScalar(prhs[1]));
//	int iX = (int)(mxGetScalar(prhs[2]));
//	int iY = (int)(mxGetScalar(prhs[3]));
//
//	// step2: get projector
//	CProjector2D* pProjector;
//	if (!(pProjector = CProjectorManager::getSingleton().get(iPid))) {
//		mexErrMsgTxt("Projector not found.\n");
//		return;
//	}
//
//	// step3: perform action
//	vector<SDetector2D> detinfo = pProjector->projectPoint(iX, iY);
//
//	// step4: output
//	if (nlhs <= 1) {
//		plhs[0] = mxCreateDoubleMatrix(detinfo.size(),	// # rows
//									   2,				// # cols
//									   mxREAL);			// datatype 32-bits
//		double* out = mxGetPr(plhs[0]);
//
//		// fill up output
//		int i = 0;
//		for (int x = 0; x < detinfo.size() ; x++) {
//			out[i] = detinfo[x].m_iAngleIndex;
//			i++;
//		}
//		for (int x = 0; x < detinfo.size() ; x++) {
//			out[i] = detinfo[x].m_iDetectorIndex;
//			i++;
//		}
//	}
//
//
//}

//-----------------------------------------------------------------------------------------
/** result = astra_mex_projector3d('is_cuda', id);
 *
 * Return is the specified projector is a cuda projector.
 * id: identifier of the projector object as stored in the astra-library.
 */
void astra_mex_projector3d_is_cuda(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{ 
	// step1: get input
	if (nrhs < 2) {
		mexErrMsgTxt("Not enough arguments.  See the help document for a detailed argument list. \n");
		return;
	}
	int iPid = (int)(mxGetScalar(prhs[1]));

	// step2: get projector
	CProjector3D* pProjector = CProjector3DManager::getSingleton().get(iPid);
	if (!pProjector || !pProjector->isInitialized()) {
		mexErrMsgTxt("Projector not initialized.\n");
		return;
	}

#ifdef ASTRA_CUDA
	CCudaProjector3D* pCP = dynamic_cast<CCudaProjector3D*>(pProjector);
	plhs[0] = mxCreateLogicalScalar(pCP ? 1 : 0);
#else
	plhs[0] = mxCreateLogicalScalar(0);
#endif
}


//-----------------------------------------------------------------------------------------
/**
 * astra_mex_projector3d('help');
 */
void astra_mex_projector3d_help(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{
	cout << "astra_mex_projector3d help:" <<endl;
	cout << "---------------------------" <<endl;
}


//-----------------------------------------------------------------------------------------

static void printHelp()
{
	mexPrintf("Please specify a mode of operation.\n");
	mexPrintf("Valid modes: create, delete, clear, get_projection_geometry,\n");
	mexPrintf("             get_volume_geometry, weights_single_ray, weights_projection\n");
	mexPrintf("				splat, is_cuda\n");
}

//-----------------------------------------------------------------------------------------
/**
 * [pid] = astra_mex_projector('read', projection_geometry, reconstruction_geometry);
 */
void mexFunction(int nlhs, mxArray* plhs[],
				 int nrhs, const mxArray* prhs[])
{
	// INPUT: Mode
	string sMode = "";
	if (1 <= nrhs) {
		sMode = mexToString(prhs[0]);	
	} else {
		printHelp();
		return;
	}

	initASTRAMex();

	// SWITCH (MODE)
	if (sMode == "create") {
		astra_mex_projector3d_create(nlhs, plhs, nrhs, prhs);
	} else if (sMode == "delete") {
		astra_mex_projector3d_destroy(nlhs, plhs, nrhs, prhs);
	} else if (sMode == "clear") {
		astra_mex_projector3d_clear(nlhs, plhs, nrhs, prhs);
	} else if (sMode == "get_projection_geometry") {
		astra_mex_projector3d_get_projection_geometry(nlhs, plhs, nrhs, prhs);
	} else if (sMode == "get_volume_geometry") {
		astra_mex_projector3d_get_volume_geometry(nlhs, plhs, nrhs, prhs);
	} else if (sMode == "weights_single_ray") {
		astra_mex_projector_weights_single_ray(nlhs, plhs, nrhs, prhs);
	//} else if (sMode == "weights_projection") {
	//	astra_mex_projector_weights_projection(nlhs, plhs, nrhs, prhs);
	//} else if (sMode == "splat") {
	//	astra_mex_projector_splat(nlhs, plhs, nrhs, prhs);
	} else if (sMode == "is_cuda") {
		astra_mex_projector3d_is_cuda(nlhs, plhs, nrhs, prhs);
	} else if (sMode == "help") {
		astra_mex_projector3d_help(nlhs, plhs, nrhs, prhs);
	} else {
		printHelp();
	}
	return;
}


