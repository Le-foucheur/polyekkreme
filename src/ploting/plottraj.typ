#set page(width: auto, height: auto, margin: 1cm)
#import "@preview/lilaq:0.6.0" as lq

#let (_, _, _, x, y, _) = lq.load-txt(
  read("../../target/pendulecoordo.csv"),
  delimiter: ";",
  skip-rows: 1,
  converters: x => x,
)

#let x = x.map(x => float(x))
#let y = y.map(x => float(x))

#figure(
  lq.diagram(
    lq.plot(x, y, smooth: true, mark: none),
  ),
  caption: [trajétoire du second pendule],
)
