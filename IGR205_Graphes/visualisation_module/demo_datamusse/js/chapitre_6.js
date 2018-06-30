let dataset = []
let width = 500
let height = 100
let barPadding = 1
let circleData = [5, 10, 15, 20, 25]

for (let i = 0; i < 26; i++) {
  let number = Math.floor(10 + Math.random() * 50)
  dataset.push(number)
}
d3
  .select('body')
  .selectAll('div')
  .data(dataset)
  .enter()
  .append('div')
  .attr('class', 'bar')
  .style('height', d => 5 * d + 'px')

let svg = d3
  .select('body')
  .append('svg')
  .attr('width', width)
  .attr('height', height)

let circles = svg
  .selectAll('circle')
  .data(circleData)
  .enter()
  .append('circle')

circles
  .attr('cx', (d, i) => i * 50 + 25)
  .attr('cy', height / 2)
  .attr('r', d => d)
  .attr('fill', 'yellow')
  .attr('stroke', 'orange')
  .attr('stroke-width', d => d / 2)

let svgBar = d3
  .select('body')
  .append('svg')
  .attr('id', 'bar')
  .attr('width', width)
  .attr('height', height)

svgBar
  .selectAll('rect')
  .data(dataset)
  .enter()
  .append('rect')
  .attr({
    'x': (d, i) => i * (width / dataset.length),
    'y': d => height - 2 * d,
    'width': Math.min(20, width / dataset.length - barPadding),
    'height': (d) => 2 * d,
    'fill': d => 'rgb(0, 0, ' + (d * 10) + ')'
  })

svgBar.selectAll('text')
  .data(dataset)
  .enter()
  .append('text')
  .text(d => d)
  .attr('x', (d, i) => (i + 0.5) * (width / dataset.length) - barPadding / 2)
  .attr('y', d => height - d * 2 + 14)
  .attr('fill', 'white')
  .attr('font-family', 'sans-serif')
  .attr('font-size', '11')
  .attr('text-anchor', 'middle')

let scatterData = []
for (let i = 0; i < 26; i++) {
  let number1 = Math.floor(Math.random() * 500)
  let number2 = Math.floor(Math.random() * 100)
  scatterData.push([number1, number2])
}
let svgScatter = d3.select('body')
  .append('svg')
  .attr('id', 'scatter')
  .attr('width', width)
  .attr('height', height)

svgScatter.selectAll('circle')
  .data(scatterData)
  .enter()
  .append('circle')
  .attr('cx', d => d[0])
  .attr('cy', d => d[1])
  .attr('r', (d, i) => Math.sqrt(d[1]))

svgScatter.selectAll('text')
  .data(scatterData)
  .enter()
  .append('text')
  .attr('x', d => d[0] + 2)
  .attr('y', d => d[1])
  .text(d => '(' + d[0] + ',' + d[1] + ')')
  .attr('fill', 'red')
  .attr('font-size', '10px')
  .attr('dominant-baseline', 'middle')
