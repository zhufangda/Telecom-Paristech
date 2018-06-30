var jsonResult;
/* url where we can make a query */
var rdfURL = ["https://ws49-cl4-jena.tl.teralab-datascience.fr/sembib/query?query=",
			  "https://ws49-cl4-jena.tl.teralab-datascience.fr/datamusee/sparql?query="];
			  
//var summuryGraphRequest = ['

/* makes an url for querying database with query */
function make_query(dataBaseID,query,addons)
{
    // URL of database where we can make SPARQL queries
    var URL = rdfURL[dataBaseID];

    // encoding the query to put in URL
    var encodedQuery = encodeURIComponent(query);

    var str =  URL + encodedQuery  + addons;

    // encodeURIComponent transforms space in %20 but we  need to transform space in + here
    return str.replace(/%20/gi,"+");
}

function make_select( selector , content , addons )
{
	return "select " + selector + " where {" + content + " } " + addons;
}

function make_graph(selector,content)
{
	return "graph" + selector + " { " + content + " } ";
}

function make_lcase(content)
{
	return "lcase(" + content + ")";
}

function make_contains(a,b)
{
	return "contains(" + a + "," + b + ")";
}

function make_filter(content)
{
	return "filter(" + content + ")";
}

function send_request(query)
{
	var request = new XMLHttpRequest();

	request.open('GET', query, true, "student" , "igr2018%");

	request.onload = function(error) {
		if (request.status >= 200 && request.status < 400) {
			// Success!
			
			//console.log( request.responseText );
			jsonResult = JSON.parse( request.responseText );
			display_data(jsonResult);

		} else {
			console.error('Could not load page.');
			console.log(request.getResponseHeader("WWW-Authenticate"));
		}
	};

	request.onerror = function(error) {
		console.error('Could not load page.');
		console.log(request.getAllResponseHeaders());
	};

	request.send();	
}
