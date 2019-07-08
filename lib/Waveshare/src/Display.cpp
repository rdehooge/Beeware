#include "Display.h"

int Display::Initialize(void)
{
  int error = _epd->Initialize();

  if (error != 0) {
    Serial.print("e-Paper init failed");
    return error;
  }

  /* This clears the SRAM of the e-paper display */
  return 0;
}

void Display::Setup(void)
{
  _epd->ClearFrame();

  _paint->Clear(UNCOLORED);
  _paint->DrawStringAt(0, 0, "Date", &Font12, COLORED);
  _epd->SetPartialWindowBlack(_paint->GetImage(), 0, 0, _paint->GetWidth(), _paint->GetHeight());

  _paint->Clear(UNCOLORED);
  _paint->DrawStringAt(0, 0, "Time", &Font12, COLORED);
  _epd->SetPartialWindowBlack(_paint->GetImage(), 0, 14, _paint->GetWidth(), _paint->GetHeight());

  _paint->Clear(UNCOLORED);
  _paint->DrawHorizontalLine(0, 0, 128, COLORED);
  _epd->SetPartialWindowBlack(_paint->GetImage(), 0, 32, _paint->GetWidth(), _paint->GetHeight());

  _paint->Clear(UNCOLORED);
  _paint->DrawStringAt(0, 0, "Voltage", &Font12, COLORED);
  _epd->SetPartialWindowBlack(_paint->GetImage(), 0, 40, _paint->GetWidth(), _paint->GetHeight());

  _paint->Clear(UNCOLORED);
  _paint->DrawStringAt(0, 0, "Charge", &Font12, COLORED);
  _epd->SetPartialWindowBlack(_paint->GetImage(), 0, 54, _paint->GetWidth(), _paint->GetHeight());

  _paint->Clear(UNCOLORED);
  _paint->DrawStringAt(0, 0, "Alert", &Font12, COLORED);
  _epd->SetPartialWindowBlack(_paint->GetImage(), 0, 68, _paint->GetWidth(), _paint->GetHeight());

  _paint->Clear(UNCOLORED);
  _paint->DrawStringAt(0, 0, "Strength", &Font12, COLORED);
  _epd->SetPartialWindowBlack(_paint->GetImage(), 0, 82, _paint->GetWidth(), _paint->GetHeight());

  _paint->Clear(UNCOLORED);
  _paint->DrawStringAt(0, 0, "Quality", &Font12, COLORED);
  _epd->SetPartialWindowBlack(_paint->GetImage(), 0, 96, _paint->GetWidth(), _paint->GetHeight());

  _paint->Clear(UNCOLORED);
  _paint->DrawHorizontalLine(0, 0, 128, COLORED);
  _epd->SetPartialWindowBlack(_paint->GetImage(), 0, 114, _paint->GetWidth(), _paint->GetHeight());

  _paint->Clear(UNCOLORED);
  _paint->DrawStringAt(0, 0, "Temperature", &Font12, COLORED);
  _epd->SetPartialWindowBlack(_paint->GetImage(), 0, 122, _paint->GetWidth(), _paint->GetHeight());

  _paint->Clear(UNCOLORED);
  _paint->DrawStringAt(0, 0, "Humidity", &Font12, COLORED);
  _epd->SetPartialWindowBlack(_paint->GetImage(), 0, 136, _paint->GetWidth(), _paint->GetHeight());

  _paint->Clear(UNCOLORED);
  _paint->DrawStringAt(0, 0, "Light", &Font12, COLORED);
  _epd->SetPartialWindowBlack(_paint->GetImage(), 0, 150, _paint->GetWidth(), _paint->GetHeight());

  _paint->Clear(UNCOLORED);
  _paint->DrawStringAt(0, 0, "Weight", &Font12, COLORED);
  _epd->SetPartialWindowBlack(_paint->GetImage(), 0, 164, _paint->GetWidth(), _paint->GetHeight());
}

void Display::Update(DeviceData &data)
{
  Setup();

  _paint->Clear(UNCOLORED);
  _paint->DrawStringAt(0, 0, Time.format(Time.local(), "%d-%h-%Y").c_str(), &Font12, COLORED);
  _epd->SetPartialWindowBlack(_paint->GetImage(), 40, 0, _paint->GetWidth(), _paint->GetHeight());

  _paint->Clear(UNCOLORED);
  _paint->DrawStringAt(0, 0, Time.format(Time.now(), "%X").c_str(), &Font12, COLORED);
  _epd->SetPartialWindowBlack(_paint->GetImage(), 40, 14, _paint->GetWidth(), _paint->GetHeight());

  String voltageString = String(data.Voltage, 2);
  int voltageWidth = voltageString.length() * Font12.Width;
  _paint->Clear(UNCOLORED);
  _paint->DrawStringAt(0, 0, voltageString.c_str(), &Font12, COLORED);
  _paint->DrawStringAt(voltageWidth, 0, "V", &Font12, COLORED);
  _epd->SetPartialWindowBlack(_paint->GetImage(), 80, 40, _paint->GetWidth(), _paint->GetHeight());

  String socString = String(data.Charge, 2);
  int socWidth = socString.length() * Font12.Width;
  _paint->Clear(UNCOLORED);
  _paint->DrawStringAt(0, 0, socString.c_str(), &Font12, COLORED);
  _paint->DrawStringAt(socWidth, 0, "%", &Font12, COLORED);
  _epd->SetPartialWindowBlack(_paint->GetImage(), 80, 54, _paint->GetWidth(), _paint->GetHeight());

  _paint->Clear(UNCOLORED);
  _paint->DrawStringAt(0, 0, String(data.Alert).c_str(), &Font12, COLORED);
  _epd->SetPartialWindowBlack(_paint->GetImage(), 80, 68, _paint->GetWidth(), _paint->GetHeight());

  String strengthString = String(data.Strength, 2);
  int strengthWidth = strengthString.length() * Font12.Width;
  _paint->Clear(UNCOLORED);
  _paint->DrawStringAt(0, 0, strengthString.c_str(), &Font12, COLORED);
  _paint->DrawStringAt(strengthWidth, 0, "%", &Font12, COLORED);
  _epd->SetPartialWindowBlack(_paint->GetImage(), 80, 82, _paint->GetWidth(), _paint->GetHeight());

  String qualityString = String(data.Quality, 2);
  int qualityWidth = qualityString.length() * Font12.Width;
  _paint->Clear(UNCOLORED);
  _paint->DrawStringAt(0, 0, qualityString.c_str(), &Font12, COLORED);
  _paint->DrawStringAt(qualityWidth, 0, "%", &Font12, COLORED);
  _epd->SetPartialWindowBlack(_paint->GetImage(), 80, 96, _paint->GetWidth(), _paint->GetHeight());

  String tempString = String(data.Temp, 2);
  int tempWidth = tempString.length() * Font12.Width;
  _paint->Clear(UNCOLORED);
  _paint->DrawStringAt(0, 0, tempString.c_str(), &Font12, COLORED);
  _paint->DrawStringAt(tempWidth, 0, "F", &Font12, COLORED);
  _epd->SetPartialWindowBlack(_paint->GetImage(), 80, 122, _paint->GetWidth(), _paint->GetHeight());

  _paint->Clear(UNCOLORED);
  _paint->DrawStringAt(0, 0, String(data.Humidity, 2).c_str(), &Font12, COLORED);
  _epd->SetPartialWindowBlack(_paint->GetImage(), 80, 136, _paint->GetWidth(), _paint->GetHeight());

  _paint->Clear(UNCOLORED);
  _paint->DrawStringAt(0, 0, String(data.Light, 2).c_str(), &Font12, COLORED);
  _epd->SetPartialWindowBlack(_paint->GetImage(), 80, 150, _paint->GetWidth(), _paint->GetHeight());

  String weightString = String(data.Weight, 2);
  int weightWidth = weightString.length() * Font12.Width;
  _paint->Clear(UNCOLORED);
  _paint->DrawStringAt(0, 0, weightString.c_str(), &Font12, COLORED);
  _paint->DrawStringAt(weightWidth, 0, "lb", &Font12, COLORED);
  _epd->SetPartialWindowBlack(_paint->GetImage(), 80, 164, _paint->GetWidth(), _paint->GetHeight());

  _epd->DisplayFrame();
  _epd->Sleep();
}
