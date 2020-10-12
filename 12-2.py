from flask import Flask
from flask import render_template


app = Flask(__name__)

@app.route('/')
def hello_world() :
    return 'Hello World!'
    
@app.route('/page')
def page() :
    return render_template("hi.html", value = "hello.py")

    
if __name__ == "__main__" :
    app.run(host = "0.0.0.0")