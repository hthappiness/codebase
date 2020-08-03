from fastapi import FastAPI
import uvicorn

app = FastAPI()

def LoadCameraMap(filePath:str):
	cameraMap = {}
	with open(filePath) as f:
		for line in f:
			infos = line.split()
			cameraMap[infos[0]] = infos[1]
	return cameraMap

	
#获取url
@app.get('/api/vms/v1/webuas/device/video/preview')
def getPreviewUrl(camera_id:str):
	cameraMap = LoadCameraMap('camerainfo.txt')
	url = ''
	if camera_id in cameraMap:
		url = cameraMap.get(camera_id)
	else:
		url = 'rtsp://192.168.11.10/smoke.mkv'
	return {'data':url}


if __name__ == "__main__":
	uvicorn.run(app,host="0.0.0.0",port=8061)
else:
	print('run on module')


