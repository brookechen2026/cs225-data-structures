/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>

#include "maptiles.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    /**
     * @todo Implement this function!
     */
    if (theSource.getRows() == 0 || theSource.getColumns() == 0 || theTiles.empty()) {
        return NULL;
    }

    vector<Point<3>> tilePoints;
    tilePoints.reserve(theTiles.size());

    map<Point<3>, TileImage*> pointToTile;

    for (size_t i = 0; i < theTiles.size(); i++) {
        LUVAPixel avg = theTiles[i].getAverageColor();
        Point<3> p = convertToXYZ(avg);

        tilePoints.push_back(p);

        if (pointToTile.find(p) == pointToTile.end()) {
            pointToTile[p] = &theTiles[i];
        }
    }

    KDTree<3> tree(tilePoints);

    MosaicCanvas* canvas = new MosaicCanvas(theSource.getRows(), theSource.getColumns());

    for (int i = 0; i < theSource.getRows(); i++) {
        for (int j = 0; j < theSource.getColumns(); j++) {
            LUVAPixel regionAvg = theSource.getRegionColor(i, j);
            Point<3> query = convertToXYZ(regionAvg);

            Point<3> bestPoint = tree.findNearestNeighbor(query);

            TileImage* chosen = pointToTile[bestPoint];
            canvas->setTile(i, j, chosen);
        }
    }

    return canvas;
}

