var numRounds = 12;

var colA = [];
var colB = [];

var humanScore = 0;
var machineScore = 0;

for (var i = 0; i < numRounds; i++) {
	if( Math.random() > 0.5 ) {
		colA.push( 1 );
	}
	else {
		colA.push( 0 );
	}
	if( Math.random() > 0.5 ) {
		colB.push( 1 );
	}
	else {
		colB.push( 0 );
	}
}

var colAOrig = colA.slice();
var colBOrig = colB.slice();



var machinePick = " ";



function setupTable() {
	var tableString = "<center> " + machinePick +"<br><table><tr><td align=center valign=top>A<br>";
	
	function rowSetup( clickFunctionName, row, i ) {
		var label = "-------";
		var c = clickFunctionName;
		if( i > 0 ) {
			c = "";
		}
		if( row == 0 ) {
			tableString += "<input type='button' value='" + label + "' style='background:blue' onclick='" + c + "' >"
		}
		else if( row == 1 ) {
			tableString += "<input type='button' value='" + label + "' style='background:red' onclick='" + c + "' > "
		}
		tableString += "<br>";
	}
	

	colA.forEach( function(r, i) { rowSetup( "colAClicked();", r, i ) } );
	
	tableString += "</td><td align=center valign=top>B<br>";

	colB.forEach( function(r, i) { rowSetup( "colBClicked();", r, i ) } );
	
	
	tableString += "</td></tr></table></center>";



	document.getElementById("content").innerHTML = 
		"<table width=100%><tr><td>Human: " + humanScore + "</td><td align=right>"+
		"Computer: " + machineScore + "</td></tr></table><br>";
	document.getElementById("content").innerHTML += tableString;
	

	document.getElementById( "content" ).innerHTML += "<br><br><br>" +
	"<input type='button' value='Rewind' onclick='rewind()' >";
}


setupTable();


function colAClicked() { 
	var v = colA.shift();
	humanScore += v;
	machinePick = " ";
	setupTable();
	timedComputerMove();
}

function colBClicked() { 
	var v = colB.shift();
	humanScore += v;
	machinePick = " ";
	setupTable();
	timedComputerMove();
}



function timedComputerMove() {
	var d = new Date();
	var startTime = d.getTime();

	pickComputerMove();
	d = new Date();
	var endTime = d.getTime();

	var elapsed = endTime - startTime;

	var targetTime = 2000;
	if( endTime - startTime < targetTime ) {
		setTimeout( setupTable, targetTime - elapsed);
	}
	else {
		setupTable();
	}
}



function pickComputerMove() {

	var v = 0;
	
	if( colA.length == 0 && colB.length == 0 ) {
		return;
	}
	else if( colA.length == 0 ) {
		v = colB.shift();
		machinePick = "Machine picks B";
	}
	else if( colB.length == 0 ) {
		v = colA.shift();	
		machinePick = "Machine picks A";
	}
	else {

		var worstScoreIfPickA = 
			worstCaseScore( colA.slice(1), colB, 0 ) + colA[0];

		var worstScoreIfPickB = 
			worstCaseScore( colA, colB.slice(1), 0 ) + colB[0];
	
		if( worstScoreIfPickA > worstScoreIfPickB ) {
			v = colA.shift();
			machinePick = "Machine picks A ( " + worstScoreIfPickA + ", " + worstScoreIfPickB + " )";

		}
		else {
			v = colB.shift();
			machinePick = "Machine picks B ( " + worstScoreIfPickA + ", " + worstScoreIfPickB + " )";
		}
	}
	machineScore += v;
}


function worstCaseScore( a, b, turn ) {
	// if we're down to only one column
	// top element goes to whoever's turn it is.
	if( a.length == 0 && b.length == 0 ) {
		return 0;
		}
	else if( a.length == 0 ) {
		if( turn ) {
			return worstCaseScore( a, b.slice(1), 0 ) + b[0];
			}
		else {
			return worstCaseScore( a, b.slice(1), 1 ) - b[0];
		}
	}
	else if( b.length == 0 ) {
		if( turn ) {
			return worstCaseScore( a.slice(1), b, 0 ) + a[0];
			}
		else {
			return worstCaseScore( a.slice(1), b, 1 ) - a[0];
		}
	}
	else {
		// neither empty
		if( turn ) {
			// our turn
			var worstIfPickA = worstCaseScore( a.slice(1), b, 0 ) + a[0];
			var worstIfPickB = worstCaseScore( a, b.slice(1), 0 ) + b[0];
			
			// we want to maximize our worst-case score after this
			if( worstIfPickA > worstIfPickB ) {
				return worstIfPickA;
			}
			else {
				return worstIfPickB;
			}
		}
		else {
			// their turn
			var worstIfPickA = worstCaseScore( a.slice(1), b, 1 ) - a[0];
			var worstIfPickB = worstCaseScore( a, b.slice(1), 1 ) - b[0];
			
			// they want to minimize our worst-case score after this
			if( worstIfPickA < worstIfPickB ) {
				return worstIfPickA;
			}
			else {
				return worstIfPickB;
			}
		}
	}
}



function rewind() {
	humanScore = 0;
	machineScore = 0;
	machinePick = "";
	colA = colAOrig.slice();
	colB = colBOrig.slice();
	setupTable();
}

//var canvas = document.createElement("canvas");
//var ctx = canvas.getContext("2d");
