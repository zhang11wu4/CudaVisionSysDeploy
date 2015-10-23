/*
 * cInitLBP.h
 *
 *  Created on: Jul 25, 2015
 *      Author: adas
 */

#ifndef CINITLBP_H_
#define CINITLBP_H_

#include "../common/detectorData.h"
#include "../utils/cudaUtils.cuh"
#include "../utils/utils.h"
#include "cMapTable.h"

class cInitLBP {
private:
	uint8_t *m_lut;
public:
	cInitLBP();
	template<typename T, typename C, typename P>
	static void initDeviceLBP(detectorData<T, C, P> *dev, dataSizes *sizes, uint pyrLevels)
	{
		cout << "initializing LBP" << endl;

		cudaMallocGen<T>(&(dev->lbp.imgDescriptor), sizes->lbp.imgDescVecElems);

		cudaMallocGen<C>(&(dev->lbp.cellHistos), sizes->lbp.cellHistosVecElems);
		cudaSafe(cudaMemset(dev->lbp.cellHistos, 0, sizes->lbp.cellHistosVecElems * sizeof(C)));

//		uchar *cell = (uchar*)malloc(sizes->cellHistosElems[0]);
//		copyDtoH(cell, dev->cellHistos, sizes->cellHistosElems[0]);
//		for (int u = 0; u < sizes->cellHistosElems[0]; u++) {
//			printf( "cell feature: %d: %d\n", u, cell[u]);
//		}

		cudaMallocGen<C>(&(dev->lbp.blockHistos), sizes->lbp.blockHistosVecElems);
		cudaMallocGen<P>(&(dev->lbp.sumHistos), sizes->lbp.sumHistosVecElems);
		cudaMallocGen<P>(&(dev->lbp.normHistos), sizes->lbp.normHistosVecElems);

		// Generate LBP mapping table
		cMapTable::generateDeviceLut(&(dev->lbp.LBPUmapTable), LBP_LUTSIZE);
	}

	template<typename T, typename C, typename P>
	static void initHostLBP(detectorData<T, C, P> *host, dataSizes *sizes, uint pyrLevels)
	{
		// Allocate memory for descriptors
		host->lbp.imgDescriptor = mallocGen<T>(sizes->lbp.imgDescVecElems);

		host->lbp.cellHistos = mallocGen<C>(sizes->lbp.cellHistosVecElems);
		memset(host->lbp.cellHistos, 0, sizes->lbp.cellHistosVecElems * sizeof(C));

		host->lbp.blockHistos = mallocGen<C>(sizes->lbp.blockHistosVecElems);
		host->lbp.sumHistos = mallocGen<P>(sizes->lbp.sumHistosVecElems);
		host->lbp.normHistos = mallocGen<P>(sizes->lbp.normHistosVecElems);

		// Generate LBP mapping table
		cMapTable::generateHostLUT(&(host->lbp.LBPUmapTable), LBP_LUTSIZE);
	}

	template<typename T, typename C, typename P>
	__forceinline__
	static void zerosCellHistogramArray(detectorData<T, C, P> *dev, dataSizes *sizes)
	{
		cudaMemset(dev->lbp.cellHistos, 0, sizes->lbp.cellHistosVecElems * sizeof(C));
		//todo evaluate asyncronous menmset or no memset(use registers)
	}

};


#endif /* CINITLBP_H_ */
