from flask import Flask, request, render_template_string, jsonify
from flask_cors import CORS  # Flask-CORS 추가

app = Flask(__name__)
CORS(app)  # CORS 설정 추가

# GPS 데이터를 저장할 리스트 (간단하게 메모리에 저장)
gps_data = []

@app.route('/')
def index():
    return "Welcome to the Flask server!"

# GET 요청과 POST 요청 모두 처리하도록 수정
@app.route('/post-data', methods=['POST'])
def post_data():
    latitude = request.form.get('latitude')
    longitude = request.form.get('longitude')
    hasTrash = request.form.get('hasTrash')

    if latitude and longitude:
        gps_data.append({"latitude": latitude, "longitude": longitude, "hasTrash": hasTrash})

        if len(gps_data) > 10:
            gps_data.pop(0)  # 최대 10개의 데이터만 유지

        print(f"Received data: Latitude={latitude}, Longitude={longitude}, hasTrash={hasTrash}")
        return jsonify({"status": "success"}), 200  # 응답을 JSON 형식으로
    else:
        return jsonify({"status": "invalid data"}), 400

# 저장된 GPS 데이터를 반환하는 엔드포인트 추가
@app.route('/gps-data', methods=['GET'])
def get_gps_data():
    return jsonify(gps_data), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
