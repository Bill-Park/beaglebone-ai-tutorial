from flask import Flask
from flask_restful import Resource, Api, reqparse

parser = reqparse.RequestParser()
parser.add_argument('name', type=str)


app = Flask(__name__)
api = Api(app)

class HelloWorld(Resource) :
    def get(self) :
        return {'hello': 'world'}

    def post(self) :
        args = parser.parse_args()
        name = args['name']
        return {'Hi': name}

api.add_resource(HelloWorld, '/')

if __name__ == '__main__' :
    app.run(debug = True, host = '0.0.0.0')