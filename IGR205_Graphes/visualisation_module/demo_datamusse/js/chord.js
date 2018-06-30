let dataset = [
    [10, 20, 30],
    [40, 60, 80],
    [100, 200, 300]
];

let chordGenerator = d3.chord();

let chords = chordGenerator(dataset);

let color = d3.scaleOrdinal(d3.schemeCategory10)
    .domain(d3.range(10))

let ribbonGenerator = d3.ribbon().radius(200);

svg = d3.select('body').append('svg').attr('width', 800).attr('height', 800)
svg.append('g').attr('transform', 'translate(' + 200 + ',' + 200+ ')')
d3.select('g')
    .selectAll('path')
    .data(chords)
    .enter()
    .append('path')
    .attr('d', ribbonGenerator)
    .attr('fill', (d,i) =>{
        console.log(d,i)
        return color(d.source.index)
})