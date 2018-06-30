let dataset = [5, 10, 15, 20, 25]

// d3.csv('data/food.csv', function (error, data) {
//   if (error != null) {
//     console.log('Error', error)
//     return
//   }
//   console.log(data)
//   dataset = data
//   draw()
// })

function draw () {
  d3.select('body')
    .selectAll('p')
    .data(dataset)
    .enter()
    .append('p')
    .style('color', 'red')
    .text((d) => ('Hello ' + d.Food))
}
