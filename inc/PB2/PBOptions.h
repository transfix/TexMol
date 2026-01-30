#ifndef _PB2Options_H_
#define _PB2Options_H_

typedef struct{
	char mesh_fname[PETSC_MAX_PATH_LEN];
	char stern_fname[PETSC_MAX_PATH_LEN];
	char quad_fname[PETSC_MAX_PATH_LEN];
	char pot_fname[PETSC_MAX_PATH_LEN];
	char pqr_fname[PETSC_MAX_PATH_LEN];
	char crd_fname[PETSC_MAX_PATH_LEN];
	char out_fname[PETSC_MAX_PATH_LEN];
	char vol_fname[PETSC_MAX_PATH_LEN];
	char vol2_fname[PETSC_MAX_PATH_LEN];
	char vol3_fname[PETSC_MAX_PATH_LEN];
	char vol4_fname[PETSC_MAX_PATH_LEN];
	char force_fname[PETSC_MAX_PATH_LEN];
	char sdf_fname[PETSC_MAX_PATH_LEN];
	PetscScalar epsilon_e; // solvent dielectric
	PetscScalar epsilon_i; // solute dielectric
	PetscScalar temperature; // temperature
	PetscScalar ionConcentration;
	PetscInt quadLevel;   // accuracy for singular quadrature
	PetscInt quadOrderFarField;   // quadrature rule to use
	PetscInt quadOrderSingular;   // quadrature rule to use
	PetscInt quadOrderNearField;   // quadrature rule to use	
	PetscInt accuracy;
	LINEAR_SOLVER_TYPE solverType;	
	BIE_FORMULATION formulation;
	DISCRETIZATION_METHOD discreteMethod;
	GEOMETRY_TYPE geometryType;
	PetscInt resolution; 
	PetscInt resolutionX; 
	PetscInt resolutionY; 
	PetscInt resolutionZ; 
	PetscScalar bboxX, bboxY, bboxZ;
	PetscScalar gridSizeX, gridSizeY, gridSizeZ;


	bool useSternLayer;

	bool computePotential;
	bool computeEnergy;
	bool computeForces;
	bool computeVolumePotential;
	bool computePotentialAtLocation;
	bool computePotentialAtAtoms;

	bool rawncOutput;

	bool sovationVolumePotentialOnly;
	bool haveSDF;

	PetscScalar dxForce;

}PBOptions;

#endif
