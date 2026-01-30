function assignRed( radius )
{
	return 1;
}

function assignBlue( radius )
{
	return 0;
}

function assignGreen( radius )
{
	return 0;
}

function getRadius( line )
{
	return 1;
}

// read a file containing the H and K values and color accordingly

var infile = new File('C:/Research/Software/Dataset/hydro/193L_c.txt.curv');
infile.open(File.ReadOnly);

var found = false;
var line = "";

while (!infile.eof) 
{
    line = infile.readLine();
	if( line.find('Number') == 0 ) 
	{
		found = true;
		print("found");
		break;
	}
}

if( found )
{
	// now find the number of vertices
	var str = line.right(line.length - 7);
	var Number numVerts = parseInt(str);

	// read HandK
	line = infile.readLine();

	print(numVerts);
	var colors = new Array( numVerts*3 );
	for ( i = 0; i < numVerts; i++ ) 
	{
		line = infile.readLine();
		radius = getRadius(line);
		colors[i*3+0] = assignRed( radius );
		colors[i*3+0] = assignBlue( radius );
		colors[i*3+0] = assignGreen( radius );
	}

	Application.surface.setVertexColors(colors,numVerts);
	print("outside");
	infile.close();
}
else
{
	print("Not found");
	infile.close();
}