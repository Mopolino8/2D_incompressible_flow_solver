#include <iostream>
#include <fstream>
#include <cmath> 
#include <assert.h> 
#include <string>


using namespace std;



//////////////////////////////////////////////////////////////

class FluidElement
{
friend class Grid;
protected:
	double pos[2];
	double psi, zeta, rhs, psiManu;



public:
	
	void set_position(double, double);
	void set_psi(double);	
	void set_zeta(double); 
	void set_rhs(double);
	void set_psiManu(double);	

	double get_psi();	
	double get_zeta();
	double get_rhs();
	double get_psiManu();
	double get_x();
	double get_y();

	void clear();
/*
	
	

*/
};


void FluidElement::set_position(double x, double y)
{
	pos[0] = x;
	pos[1] = y;
}

void FluidElement::set_psi(double psiSpec)
{
	psi = psiSpec;
}

void FluidElement::set_zeta(double zetaSpec)
{
	zeta = zetaSpec;
}

void FluidElement::set_rhs(double rhsSpec)
{
	rhs = rhsSpec;
}

void FluidElement::set_psiManu(double psiManuSpec)
{
	psiManu = psiManuSpec;
}
void FluidElement::clear()
{

	psi  = 0;
	zeta = 0;
	rhs  = 0;
	psiManu  = 0;
}




double FluidElement::get_psi()
{
	return psi;
}

double FluidElement::get_zeta()
{
	return zeta;
}

double FluidElement::get_rhs()
{
	return rhs;
}

double FluidElement::get_psiManu()
{
	return psiManu;
}

double FluidElement::get_x()
{
	return pos[0];
}

double FluidElement::get_y()
{
	return pos[1];
}


/////////////////////////////////////////////////////////////////




class Grid
{


protected:
	int nx , ny;
	double dx, dy, l, w;
	double residualPsi[2];	
	FluidElement nodes[200][200];

public:

	Grid(int,int,double,double);
	FluidElement *get_node(int,int);
	void set(std::string,int,int,double);
	double get(std::string,int,int);
	double get(std::string);	
//	void calc_residualPsi();
	int solve_poisson(double,double);

	


};



Grid::Grid(int nxSpec,int nySpec, double lSpec, double wSpec)
{

	assert(max(nx,ny) <= 200);
	

	nx = nxSpec;
	ny = nySpec;

	l  = lSpec;
	w  = wSpec;

	dx   = l/(nx - 1);
	dy   = w/(ny - 1);

	

	for (int k = 0 ; k <= nx - 1 ; k++)
	{
		for (int l = 0 ; l <= ny - 1 ; l++)
		{
			nodes[k][l].clear();
			double x = k*dx;
			double y = l*dy;
			nodes[k][l].set_position(x, y);
		}
	}

}

FluidElement *Grid::get_node(int k, int l)
{
	return &nodes[k][l];
}


void Grid::set(std::string var, int k, int l, double val)
{

if(var.compare("psi") == 0)	
{
	nodes[k][l].set_psi(val);
}


else if(var.compare("zeta") == 0)	
{
	nodes[k][l].set_zeta(val);
}


else if(var.compare("rhs") == 0)	
{
	nodes[k][l].set_rhs(val);	
}

else if(var.compare("psiManu") == 0)	
{
	nodes[k][l].set_psiManu(val);	
}

else 
{
	cout << "Invalid get_var selection (select: psi,zeta,rhs, psiManu) "  << 	endl;
}

}





double Grid::get(std::string var, int k, int l)
{


double val;

if(var.compare("psi") == 0)	
{
	val = nodes[k][l].get_psi();
}


else if(var.compare("zeta") == 0)	
{
	val = nodes[k][l].get_zeta();
}


else if(var.compare("rhs") == 0)	
{
	val = nodes[k][l].get_rhs();	
}

else if(var.compare("psiManu") == 0)	
{
	val = nodes[k][l].get_psiManu();	
}

else if(var.compare("x") == 0)	
{
	val = nodes[k][l].get_x();	
}

else if(var.compare("y") == 0)	
{
	val = nodes[k][l].get_y();	
}

else 
{
	cout << "Invalid get_var selection (select: psi,zeta,rhs,psiManu,x,y) "  << 	endl;
	val = -9999.999;
}

return val;

}

double Grid::get(std::string var)
{


double val;

if(var.compare("length") == 0)		
{
	val = l;	
}

else if(var.compare("width") == 0)	
{
	val = w;	
}

else if(var.compare("dx") == 0)	
{
	val = dx;	
}

else if(var.compare("dy") == 0)	
{
	val = dy;	
}

else 
{
	cout << "Invalid get_var selection (select: length,width,dx,dy) "  << 	endl;
	val = -9999.999;
}

return val;

}





/*


void Grid::calc_residualPsi()
{
// initialize residualPsi and counter
double residualPsiCum = 0.0;
double residualPsiI = 0.0;
double residualPsiMax = 0.0;
int n = 0;

	for (int k = 1 ; k <= nx - 2 ; k++)
	{
		for (int l = 1 ; l <= ny - 2 ; l++)
		{
		n += 1;
		residualPsiI = abs(nodes[k][l].get_rhs() - nodes[k][l].get_psiManu());
		if (residualPsiI > residualPsiMax)
		 	{
			residualPsiMax = residualPsiI;
			}
		residualPsiCum += residualPsiCum;
		}
	}

residualPsi[0] = residualPsiMax;
residualPsi[1] = residualPsiCum/n;
}

*/


int Grid::solve_poisson(double tol,double omega)
{
//nx = 10;
//ny = 10;
// initialize residualPsis and iterations
	double residualPsiMax = tol;
	double residualPsiCum = 0;
	double residualPsiI   = 0;
	int iterations = 0;

/*
	(Grid::calc_residualPsi();
	int iterations = 0;
*/	
	

// Initialize Stensil Variables
	double beta = dx/dy;	
	double psi_new = 0;
	double psikl   = 0;	
	double psikp1l = 0;
	double psikm1l = 0;	
	double psiklp1 = 0;
	double psiklm1 = 0;
	double rhs  = 0;


	while (residualPsiMax >= tol)	
	{	
		// reset residualPsi to find new max
		residualPsiMax = 0;		
		iterations += 1;
		
		// loop through interior points
		for (int k = 0 ; k <= nx - 1 ; k++)
		{
			for (int l = 0 ; l <= ny - 1 ; l++)
			{

			psikl   = Grid::get("psi",k,l);	
			psikp1l = Grid::get("psi",k+1,l);
			psikm1l = Grid::get("psi",k-1,l);	
			psiklp1 = Grid::get("psi",k,l+1);
			psiklm1 = Grid::get("psi",k,l-1);
			rhs  = Grid::get("rhs",k,l);

			psi_new = ( (psikp1l + psikm1l) + (psiklp1 + psiklm1)*beta*beta - rhs*dx*dx)/(1 + beta*beta)/2;

			residualPsiI = abs(psikl - psi_new); // residualPsi should acually be calculated after all psi_new calc
				
			if (residualPsiI > residualPsiMax)
			{
				residualPsiMax = residualPsiI;
			}
				
			cout << residualPsiI << endl;
			psi_new = (1-omega)*psikl + omega*psi_new;
			Grid::set("psi",k,l,psi_new);
			}		
	
		}
//		Grid::calc_residualPsi();
	}
	residualPsi[0] = residualPsiMax;
	residualPsi[1] = residualPsiCum/iterations;
	return iterations;
}





///////////////////////////////////////////////////////////









int main()
{

double pi = atan(1)*4;

int nx, ny;
nx = 10;
ny = 10;
double length, width, psiTbc, psiBbc,psiLbc, psiRbc;
length = 1;
width = 1;
psiTbc = psiBbc = psiLbc = psiRbc = 0;

Grid grid(nx,ny,length,width);

//std::string str = "psiManu";

//test.set(str,2,2,432.5);
//double psi = test.get(str,2,2);

// initial conditions



double L = grid.get("length");
double W = grid.get("width");
	
double xkl, ykl, psiManu, rhs, psiInitial;

for (int k = 1 ; k <= nx - 2 ; k++)
{
	for (int l = 1 ; l <= ny - 2 ; l++)
	{


	if ( k == 0)
		{
		grid.set("psi",k,l,psiLbc);
		}

		else if ( k == nx - 1)
		{
		grid.set("psi",k,l,psiRbc);
		}
			
		else if ( l == 0)
		{
		grid.set("psi",k,l,psiBbc);
		}

		else if ( l == ny - 1)
		{
		grid.set("psi",k,l,psiTbc);
		}
			
		else 
		{
		xkl     = grid.get("x",k,l);
		ykl     = grid.get("y",k,l);

		
		psiManu = sin(xkl*2*pi/L)*sin(ykl*2*pi/W); // this is a fake solution to psi
		grid.set("psiManu",k,l,psiManu);
		
		rhs = -4*pi*pi*sin(xkl*2*pi/L)*sin(ykl*2*pi/W)*(1/L/L + 1/W/W);
		grid.set("rhs",k,l,rhs);

		psiInitial = cos(xkl*2*pi/L)*cos(ykl*2*pi/W);
		grid.set("psi",k,l,psiInitial);
		}
	}
}

/*

// left boundary ( k = 0 )
new int k = 0;	
	for (new int l = 0 ; l <= ny - 1 ; l++)



// right boundary (k = nx-1 )
new int k = nx - 1;	
	for (new int l = 0 ; l <= ny - 1 ; l++)
	{
	grid.set("psi",k,l,psiRbc);
	}


// top boundary ( l = 0 )
new int l = 0;	
	for (new int k = 0 ; k <= nx - 1 ; k++)
	{
	grid.set("psi",k,l,psiTbc);
	}


// top boundary ( l = ny - 1 )
new int l = ny - 1;	
	for (new int k = 0 ; k <= nx - 1 ; k++)
	{
	grid.set("psi",k,l,psiBbc);
	}

*/

double tol = pow(10,-4);
double omega = 0.9;
grid.solve_poisson(tol,omega);



/*


FluidElement *p;
p = test.get_node(1,1);

(*p).set_psi(12.3);
double psi = (*p).get_psi();


	for (int k = 0 ; k <= nx - 1 ; k++)
	{
		for (int l = 0 ; l <= ny - 1 ; l++)
		{
		
		
		

		}
	}
*/


return 0;
}

