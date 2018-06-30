let scatterData = []
let width = 800
let height = 800

for (let i = 0; i < 26; i++) {
  let number1 = Math.floor(Math.random() * 500)
  let number2 = Math.floor(Math.random() * 100)
  scatterData.push([number1, number2])
}

let minX = d3.min(scatterData, d => d[0])
let maxX = d3.max(scatterData, d => d[0])
let minY = d3.min(scatterData, d => d[1])
let maxY = d3.max(scatterData, d => d[1])
let padding = 30

let x = d3.scale.linear()
  .domain([minX, maxX])
  .rangeRound([padding, width - padding * 2])

let y = d3.scale.linear()
  .domain([minY, maxY])
  .range([height - padding, padding])
  .nice()

let svg = d3.select('body')
  .append('svg')
  .attr('height', height)
  .attr('width', width)

svg.selectAll('circle')
  .data(scatterData)
  .enter()
  .append('circle')
  .attr('cx', d => x(d[0]))
  .attr('cy', d => y(d[1]))
  .attr('r', (d, i) => i)

svg.selectAll('text')
  .data(scatterData)
  .enter()
  .append('text')
  .attr('x', d => x(d[0]))
  .attr('y', d => y(d[1]))
  .attr('fill', 'red')
  .attr('font-size', 18)
  .text(d => '(' + d[0] + ',' + d[1] + ')')

// x axis
let xAxis = d3.svg.axis()
  .scale(x)
  .orient('bottom')
  .ticks(10)

svg.append('g')
  .attr('class', 'axis')
  .attr('transform', 'translate(0,' + (height - padding) + ')')
  .call(xAxis)

// y axis
let yAxis = d3.svg.axis()
  .scale(y)
  .orient('left')
  .ticks(5)

svg.append('g')
  .attr('class', 'axis')
  .attr('transform', 'translate(' + padding + ',0)')
  .call(yAxis)
