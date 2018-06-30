
let height = 800, width = 1800
let color = d3.scaleOrdinal(d3.schemeCategory10)
let dataset, scaleR


let svg = d3.select("body").append('svg')
    .attr('width', width)
    .attr('height', height)
let links = svg.append("g").attr('class','links').selectAll('line')
let nodes = svg.append('g').attr('class','nodes').selectAll('circle')

let simulation = d3.forceSimulation()
    .force("link", d3.forceLink().id(function(d) { return d.id; }))
    .force("charge", d3.forceCollide(20))
    .force("center", d3.forceCenter(width / 2, height / 2))

function updateScale(){
    let max = d3.max(dataset.nodes, d =>d.size)
    let min = d3.min(dataset.nodes, d => d.size)

    console.log('min:', min, 'max', max)
    scaleR = d3.scaleLinear().range([5,25])
        .domain([min, max])
}


function updateGraph(graph) {
    dataset = graph
    updateScale()
    links = links.data(dataset.links)
    nodes = nodes.data(dataset.nodes, d=> d.id)

    links.exit()
        .transition()
        .duration(300)
        .attr("stroke-width", 0)
        .remove()

    nodes.exit()
        .transition().duration(500)
        .attr("r", 0)
        .attr("fill", function(d) { return color(d.group); })
        .remove()



    links = links.enter().append("line")
        .attr("stroke-width", 2)
        .attr('stroke', 'black')
        .merge(links)


    nodes = nodes.enter()
        .append("circle")
        .attr('r', d=> scaleR(d.size))
        .attr("fill", function(d) { return color(d.group); })
        .on('click', (d) => {
            console.log(d)
            d3.json('data/' + d.file)
                .then(
                    updateGraph
                )
        }).call(d3.drag()
            .on("start", dragstarted)
            .on("drag", dragged)
            .on("end", dragended))
        .merge(nodes)

    nodes.transition()
        .duration(1000)
        .attr('r', d=>scaleR(d.size))



    nodes.append("title")
        .text(function(d) { return d.id; });

    simulation
        .nodes(dataset.nodes)
        .on("tick", ticked);

    simulation.force("link")
        .links(dataset.links);


    simulation.alphaTarget(0.1).restart();
}



d3.json('data/total_G.json').then(
    data => {
        ori_data = data
        updateGraph(data)
    })


function dragstarted(d) {
    if (!d3.event.active) simulation.alphaTarget(0.1).restart();
    d.fx = d.x;
    d.fy = d.y;
}

function dragged(d) {
    d.fx = d3.event.x;
    d.fy = d3.event.y;
}

function dragended(d) {
    if (!d3.event.active) simulation.alphaTarget(0);
    d.fx = null;
    d.fy = null;
}


function ticked() {
    links
        .attr("x1", function(d) { return d.source.x; })
        .attr("y1", function(d) { return d.source.y; })
        .attr("x2", function(d) { return d.target.x; })
        .attr("y2", function(d) { return d.target.y; });

    nodes
        .attr("cx", function(d) { return d.x; })
        .attr("cy", function(d) { return d.y; });
}


d3.select('#back').on('click', function(){
    d3.json('data/total_G.json').then(
        data => {
            updateGraph(data)
        })
})
