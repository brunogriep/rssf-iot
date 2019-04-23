import dash
import dash_core_components as dcc
import dash_html_components as html
import serial

external_stylesheets = ['https://codepen.io/chriddyp/pen/bWLwgP.css']

app = dash.Dash(__name__, external_stylesheets=external_stylesheets)

with serial.Serial('/dev/ttyACM0', 115200, timeout=1, ) as ser:
        line = ser.readline()   # read a '\n' terminated line

app.layout = html.Div(children=[
    html.H1(children='Sai daqui Ayrton'),

    html.Div(children='''
        Dash: Desafio aula 6.
    '''),

    dcc.Graph(
        id='example-graph',
        figure={
            'data': [
                {'x': [line, 2, 3], 'y': [4, 1, 2], 'type': 'bar', 'name': 'SF'},
                {'x': [1, 2, 3], 'y': [2, 4, 5], 'type': 'bar', 'name': u'Mont'},
            ],
            'layout': {
                'title': 'Plot Temperatura'
            }
        }
    )
])



if __name__ == '__main__':
    app.run_server(debug=True)