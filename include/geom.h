#ifndef __GEOM_H__
#define __GEOM_H__

typedef struct _PedGeometry PedGeometry;

/* Geometry of the partition */
struct _PedGeometry {
	PedDevice *dev;
	PedSector start;
	PedSector length;
	PedSector end;
};


#endif
