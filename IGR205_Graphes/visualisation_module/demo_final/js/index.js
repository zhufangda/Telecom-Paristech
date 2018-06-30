/* An array containing the keywords to search for */
var keywords;

/* the graph in which we want to search */
var graph = 0;

/* the seleceted visualisation */
var visualisation;

/* the input for the keywords */
var rdf_search_input = document.getElementById("rdf-search-input");
rdf_search_input.addEventListener("keypress" , lauchSearchOnkeypress );

var rdf_search_button = document.getElementById("rdf-search-button");
rdf_search_button.addEventListener("click" , launchSearchClick );

function lauchSearchOnkeypress(event)
{
	if( event.keyCode == 13 )
		launchSearch();
}
function launchSearchClick(){ launchSearch(); }

/* the function that is called when the user press enter or the search button */
function launchSearch()
{
	keywords = retrieveKeywords();
	console.log( keywords );
	var query = "?s1 a ?t1 .";
	
	for( var i=0 ; i < keywords.length ; i++ )
		query += make_filter( make_contains(make_lcase("str(?s1)") , make_lcase('"' + keywords[i] + '"') ) );
	
	query += "?s1 ?p ?s2 ." + make_filter( "isIri(?s2)" ) + make_filter( "?s1!=?s2" ) + "?s2 a ?t2 ." + make_filter("?t1!=?t2");
	query = make_select( "distinct ?t1 ?p ?t2" , make_graph( "?g" , query ) , "LIMIT 100" );
	
	console.log(query);
	console.log( make_query( graph, query , "") );
	send_request( make_query( graph, query , "") );
}

/* retrieves all keywords from the input tag and returns the array */
function retrieveKeywords()
{
	var container = new Array();
	var keywords = rdf_search_input.value;
	var containerTmp = keywords.split(" ");
	
	var length = containerTmp.length;
	for( var i=0 ; i < length  ; i++ )
	{
		if( containerTmp[i] != "" )
			container.push( containerTmp[i] );
	}
	
	return container;
}

/* changes the selected graph */

var selectGraphBox = document.getElementById("selectGraph");
selectGraphBox.addEventListener( "change" , change_graph );

function change_graph() {
    var selectBox = document.getElementById("selectGraph");
    graph = selectBox.options[selectBox.selectedIndex].value;
}



