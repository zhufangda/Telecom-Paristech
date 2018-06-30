# Don't modify this file

from urllib.parse import urlencode
from urllib.request import Request, urlopen
import json

from IPython.display import display
import ipywidgets as widgets
from traitlets import Unicode, validate
from getpass import getuser

class Connector(object):
    def __init__(self, base_url, lab_id, student_id):
        self.base_url = base_url
        self.lab_id = lab_id
        self.student_id = student_id
        self.user = getuser()

    def get_question(self, question_id):
        post_fields = {'student_id': self.student_id, 'question_id': question_id, 'user': self.user}
        url = "{}/{}/{}".format(self.base_url, self.lab_id, 'question')
        request = Request(url, urlencode(post_fields).encode())
        server_response = json.loads(urlopen(request).read().decode())
        if "message" in server_response:
            print(server_response["message"])
        if "arguments" in server_response:
            return server_response["arguments"]

    def post_answer(self, question_id, answer):
        post_fields = {'student_id': self.student_id, 'question_id': question_id,
                       'answer': answer, 'user': self.user}
        url = "{}/{}/{}".format(self.base_url, self.lab_id, 'question')
        request = Request(url, urlencode(post_fields).encode())
        server_response = json.loads(urlopen(request).read().decode())
        print(server_response["message"])

    def get_answer(self, question_id, review_id):
        post_fields = {'action': 'get',
                       'student_id': self.student_id,
                       'question_id': question_id,
                       'review_id': review_id}
        url = "{}/{}/{}".format(self.base_url, self.lab_id, 'review')
        request = Request(url, urlencode(post_fields).encode())
        server_response = json.loads(urlopen(request).read().decode())
        if "message" in server_response:
            print(server_response["message"])
        if "answers" in server_response:
            print(server_response["answers"])

    def post_rating(self, question_id, review_id, rating):
        post_fields = {'action': 'post',
                       'user': self.user,
                       'student_id': self.student_id,
                       'question_id': question_id,
                       'review_id': review_id,
                       'rating': rating}
        url = "{}/{}/{}".format(self.base_url, self.lab_id, 'review')
        request = Request(url, urlencode(post_fields).encode())
        server_response = json.loads(urlopen(request).read().decode())
        print(server_response["message"])

    def post_text(self, question_id):
        text = widgets.Textarea(value="...")
        button = widgets.Button(description="Send")
        display(text)
        display(button)

        def handle_submit(sender):
            answer = text.value
            post_fields = {'student_id': self.student_id, 'question_id': question_id,
                           'answer': answer, 'user': self.user}
            url = "{}/{}/{}".format(self.base_url, self.lab_id, 'question')
            request = Request(url, urlencode(post_fields).encode())
            server_response = json.loads(urlopen(request).read().decode())
            print(server_response["message"])

        button.on_click(handle_submit)



