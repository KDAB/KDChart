TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = tools \
          Axis \
          Background \
          Bars \
          DataValueTexts \
          demo \
          DrawIntoPainter \
          EmptyValues \
          Gantt \
          Grids \
          HeadersFooters \
          Legends \
          LeveyJennings \
          Lines \
          ModelView \
          NoValues \
          Pie \
          Plane \
          Plotter \
          Polar \
          RealTime \
          SharedAbscissa \
          Stock \
          TernaryCharts \
          Widget \
          Zoom

!wasm: {
    SUBDIRS += Sql
}
