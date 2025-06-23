from fastapi import FastAPI, Request
from fastapi.responses import HTMLResponse
from fastapi.templating import Jinja2Templates
from fastapi.responses import JSONResponse

app = FastAPI()

templates = Jinja2Templates(directory="templates")

sensors = {}

@app.get("/api")
async def api():
    return JSONResponse(content=sensors)

@app.post("/api")
async def api(request: dict):
    message = {
        "message": "Data sent successfully."
    }

    try:
        sensors["button_a"] = request.get("button_a", None)
        sensors["button_b"] = request.get("button_b", None)
        sensors["joystick_x"] = request.get("joystick_x", None)
        sensors["joystick_y"] = request.get("joystick_y", None)
        sensors["wind_rose"] = request.get("wind_rose", None)
        sensors["temperature"] = request.get("temperature", None)
    except Exception:
        message = {
            "message": "There was an error sending the data. Check that the variables are correct."
        }

    return JSONResponse(content=message)

@app.get("/", response_class=HTMLResponse)
async def home(request: Request):
    context = {
        "request": request,
    }

    return templates.TemplateResponse("index.html", context)