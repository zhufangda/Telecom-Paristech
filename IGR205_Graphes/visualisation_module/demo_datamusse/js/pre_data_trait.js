let graphe = new jsnx.Graph()

let dataset = []

d3.json("data/total_G.json")
    .then( trait_data);


function trait_data(data){
    console.log(data)
}

