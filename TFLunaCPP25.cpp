// C++ TF-Luna Program
#include <wx/wx.h>
#include <wx/statbox.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <gpiod.h>
#include <linux/i2c-dev.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <string>
#include <sstream>
#include <iomanip>
#include </home/dakserver/Desktop/TFluna/MdlTFLuna25.h>
 
class MyTFLunaApp : public wxApp
{
public:
    bool OnInit() override;
};
 
wxIMPLEMENT_APP(MyTFLunaApp);
 
class MyForm : public wxFrame
{
public:
    MyForm(); //Add constructor declaration
    ~MyForm(); // Add destructor declaration
 
private:
    void OnTFLuna(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnDistanceButton(wxCommandEvent& event);
    void OnTemperatureButton(wxCommandEvent& event);
    void OnAmplitudeButton(wxCommandEvent& event);
    void OnGetRateButton(wxCommandEvent& event);
    void OnFirmwareButton(wxCommandEvent& event);
    void OnSerialNumberButton(wxCommandEvent& event);
    void OnTicksButton(wxCommandEvent& event);
    void OnErrorCodeButton(wxCommandEvent& event);
    void OnOpCodeButton(wxCommandEvent& event);
    void OnPowerModeButton(wxCommandEvent& event);
    void OnGetStatusButton(wxCommandEvent& event);
    void OnAmpThresholdButton(wxCommandEvent& event);
    void OnDummyDistButton(wxCommandEvent& event);
    void OnMinDistanceButton(wxCommandEvent& event);
    void OnMaxDistanceButton(wxCommandEvent& event);
    wxTextCtrl *textArea;
    wxStaticText *label1;
    void OnBtnChangeAddress(wxCommandEvent& event);
    wxTextCtrl *TbAddress;
    wxTextCtrl *TbSetAddress;
    void OnBtnSetI2CAddress(wxCommandEvent& event);
    wxTextCtrl *TbMinDistance;
    wxTextCtrl *TbMaxDistance;
    void OnBtnSetDistanceLimits(wxCommandEvent& event);
    // TFLuna sensor instance - constructor will be called automatically
    MdlTFLuna* TFLuna;
    void OnBtnReset(wxCommandEvent& event);
};
 
enum
{
    ID_TFLuna = 1,
    ID_DistanceButton,
    ID_TemperatureButton,
    ID_AmplitudeButton,
    ID_GetRateButton,
    ID_FirmwareButton,
    ID_SerialNumberButton,
    ID_TicksButton,
    ID_ErrorCodeButton,
    ID_OpCodeButton,
    ID_PowerModeButton,
    ID_GetStatusButton,
    ID_AmpThresholdButton,
    ID_DummyDistButton,
    ID_MinDistanceButton,
    ID_MaxDistanceButton,
    ID_GBGetInfo, 
    ID_GBChangeI2CAddr,
    ID_LblChangeI2CAddr,
    ID_LblNewAddress,
    ID_TbAddress,
    ID_BtnChangeAddress,
    ID_GBSetI2CAddr,
    ID_LblSetAddress,
    ID_LblSetCurrentAddress,
    ID_TbSetAddress,
    ID_BtnSetI2CAddress,
    ID_GBSetDistLimits,
    ID_LblMinimumDistance,
    ID_TbMinDistance,
    ID_LblMinDistcm,
    ID_TbMaxDistance,
    ID_LblMaxDistcm,
    ID_BtnSetDistanceLimits,
    ID_GBReset,
    ID_BtnReset
};
 
bool MyTFLunaApp::OnInit()
{
    MyForm *form = new MyForm();
    form->Show(true);
    return true;
}
 
MyForm::MyForm()
    : wxFrame(nullptr, wxID_ANY, "TF-Luna", wxDefaultPosition, wxSize(776, 650)),
      TFLuna(nullptr)  // Initialize pointer to nullptr
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_TFLuna, "&TFLuna\tCtrl-H",
                     "Press a button to get distance, etc.");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
 
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
 
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
 
    SetMenuBar(menuBar);
 
    CreateStatusBar();
    SetStatusText("TF-Luna Program");
    ///////////////////////
    // Create main panel //
    ///////////////////////
    wxPanel *panel = new wxPanel(this, wxID_ANY);
    
    ///////////////////
    //Create a label //
    ///////////////////
    label1 = new wxStaticText(panel, wxID_ANY, "LiDAR I2C Measurement", wxPoint(20, 0), wxSize(560, 20), wxALIGN_CENTER_HORIZONTAL);
                            
    ////////////////////////////////////////////////////
    // Create TextArea at specified position and size //
    ////////////////////////////////////////////////////
    textArea = new wxTextCtrl(panel, wxID_ANY, "",
                              wxPoint(20, 40),
                              wxSize(568, 169),
                              wxTE_MULTILINE | wxTE_READONLY);
    
    /////////////////////////////////////////////////////////
    // Create StaticBox (GroupBox) on the right - Get Info //
    /////////////////////////////////////////////////////////
    wxStaticBox *GBGetInfo = new wxStaticBox(panel, ID_GBGetInfo, "Get Info", wxPoint(603, 25), wxSize(153,490));
    
    // Create button inside the group box. Use Raise() to ensure the button appears above the static box in the z-order
    wxButton *distanceButton = new wxButton(panel, ID_DistanceButton, "Distance", wxPoint(620, 50), wxSize(120, 26));
    distanceButton->Raise();
    wxButton *temperatureButton = new wxButton(panel, ID_TemperatureButton, "Temperature", wxPoint(620, 80), wxSize(120, 26));
    temperatureButton->Raise();
    wxButton *amplitudeButton = new wxButton(panel, ID_AmplitudeButton, "Amplitude", wxPoint(620, 110), wxSize(120, 26));
    amplitudeButton->Raise();
    wxButton *getrateButton = new wxButton(panel, ID_GetRateButton, "Get Rate", wxPoint(620, 140), wxSize(120, 26));
    getrateButton->Raise();
    wxButton *firmwareButton = new wxButton(panel, ID_FirmwareButton, "Firmware", wxPoint(620, 170), wxSize(120, 26));
    firmwareButton->Raise();
    wxButton *serialnumberButton = new wxButton(panel, ID_SerialNumberButton, "Serial Number", wxPoint(620, 200), wxSize(120, 26));
    serialnumberButton->Raise();
    wxButton *ticksButton = new wxButton(panel, ID_TicksButton, "Ticks", wxPoint(620, 230), wxSize(120, 26));
    ticksButton->Raise();
    wxButton *errorcodeButton = new wxButton(panel, ID_ErrorCodeButton, "Error Code", wxPoint(620, 260), wxSize(120, 26));
    errorcodeButton->Raise();
    wxButton *opcodeButton = new wxButton(panel, ID_OpCodeButton, "Op Mode", wxPoint(620, 290), wxSize(120, 26));
    opcodeButton->Raise();
    wxButton *powermodeButton = new wxButton(panel, ID_PowerModeButton, "Power Mode", wxPoint(620, 320), wxSize(120, 26));
    powermodeButton->Raise();
    wxButton *getstatusButton = new wxButton(panel, ID_GetStatusButton, "Get Status", wxPoint(620, 350), wxSize(120, 26));
    getstatusButton->Raise();
    wxButton *ampthresholdButton = new wxButton(panel, ID_AmpThresholdButton, "Amp Threshold", wxPoint(620, 380), wxSize(120, 26));
    ampthresholdButton->Raise();
    wxButton *dummydistButton = new wxButton(panel, ID_DummyDistButton, "Dummy Dist", wxPoint(620, 410), wxSize(120, 26));
    dummydistButton->Raise();
    wxButton *mindistanceButton = new wxButton(panel, ID_MinDistanceButton, "Min Distance", wxPoint(620, 440), wxSize(120, 26));
    mindistanceButton->Raise();
    wxButton *maxdistanceButton = new wxButton(panel, ID_MaxDistanceButton, "Max Distance", wxPoint(620, 470), wxSize(120, 26));
    maxdistanceButton->Raise();
    
    //////////////////////////////////////////////////////////////////////////////
    // Create StaticBox (GroupBox) on the Left-mid - Get Info, ChangeI2CAddress //
    //////////////////////////////////////////////////////////////////////////////
    wxStaticBox *GBChangeI2CAddr = new wxStaticBox(panel, ID_GBChangeI2CAddr, "Change I2C Addr", wxPoint(20, 220), wxSize(288,161));
    
    //Create a label
    wxStaticText *LblChangeI2CAddr = new wxStaticText(panel, ID_LblChangeI2CAddr, "Default Address 0x10", wxPoint(30, 250), wxSize(248, 20), wxALIGN_CENTER_HORIZONTAL);
    LblChangeI2CAddr->Raise();
    
    //Create another label
    wxStaticText *LblNewAddress = new wxStaticText(panel, ID_LblNewAddress, "New Address:", wxPoint(50, 290), wxSize(248, 20), wxALIGN_LEFT);
    LblChangeI2CAddr->Raise();
    
    //Create a TextBox (TextControl)
    TbAddress = new wxTextCtrl(panel, ID_TbAddress, "", wxPoint(170, 290), wxSize(80, 25), wxALIGN_RIGHT);
    TbAddress->Raise();
    
    //Create a button
    wxButton *BtnChangeAddress = new wxButton(panel, ID_BtnChangeAddress, "OK", wxPoint(120, 330), wxSize(80, 26));
    BtnChangeAddress->Raise();
    
    //////////////////////////////////////////////////////////////////////////
    // Create StaticBox (GroupBox) on the Mid-mid - Get Info, SetI2CAddress //
    //////////////////////////////////////////////////////////////////////////
    wxStaticBox *GBSetI2CAddr = new wxStaticBox(panel, ID_GBSetI2CAddr, "Set I2C Addr", wxPoint(320, 220), wxSize(268,161));
    
    //Create a label
    wxStaticText *LblSetI2CAddr = new wxStaticText(panel, ID_LblChangeI2CAddr, "Default Address: 0x10", wxPoint(300, 250), wxSize(228, 20), wxALIGN_RIGHT);
    LblSetI2CAddr->Raise();
    
    //Create another label
    wxStaticText *LblSetCurrentAddress = new wxStaticText(panel, ID_LblSetCurrentAddress, "Set Current Addr::", wxPoint(220, 290), wxSize(248, 20), wxALIGN_RIGHT);
    LblSetCurrentAddress->Raise();
    
    //Create a TextBox (TextControl)
    TbSetAddress = new wxTextCtrl(panel, ID_TbSetAddress, "", wxPoint(480, 290), wxSize(80, 25), wxALIGN_RIGHT);
    TbAddress->Raise();
    
    //Create a button
    wxButton *BtnSetI2CAddress = new wxButton(panel, ID_BtnSetI2CAddress, "OK", wxPoint(400, 330), wxSize(80, 26));
    BtnSetI2CAddress->Raise();
    
    //////////////////////////////////////////////////////////////////////////
    // Create StaticBox (GroupBox) on the Mid-low - Set Dist Limits        //
    //////////////////////////////////////////////////////////////////////////
    wxStaticBox *GBSetDistLimits = new wxStaticBox(panel, ID_GBSetDistLimits, "Set Dist Limits", wxPoint(20, 390), wxSize(288,161));
    
    //Create another label
    wxStaticText *LblMinDist = new wxStaticText(panel, ID_LblMinimumDistance, "Minimum:", wxPoint(40, 430), wxSize(100, 20), wxALIGN_RIGHT);
    LblMinDist->Raise();
    
    //Create a TextBox (TextControl)
    TbMinDistance = new wxTextCtrl(panel, ID_TbMinDistance, "", wxPoint(150, 430), wxSize(80, 25), wxALIGN_LEFT);
    TbMinDistance->SetValue("0");
    TbMinDistance->Raise();
    
    //Create another label
    wxStaticText *LblMinDistcm = new wxStaticText(panel, ID_LblMinimumDistance, "cm", wxPoint(165, 430), wxSize(100, 20), wxALIGN_RIGHT);
    LblMinDistcm->Raise();
    
     //Create another label
    wxStaticText *LblMaxDist = new wxStaticText(panel, ID_LblMinimumDistance, "Maximum:", wxPoint(40, 460), wxSize(100, 20), wxALIGN_RIGHT);
    LblMinDist->Raise();
    
    //Create a TextBox (TextControl)
    TbMaxDistance = new wxTextCtrl(panel, ID_TbMinDistance, "", wxPoint(150, 460), wxSize(80, 25), wxALIGN_LEFT);
    TbMaxDistance->SetValue("9000");
    TbMaxDistance->Raise();
    
    //Create another label
    wxStaticText *LblMaxDistcm = new wxStaticText(panel, ID_LblMinimumDistance, "cm", wxPoint(165, 460), wxSize(100, 20), wxALIGN_RIGHT);
    LblMinDistcm->Raise();
    
    //Create a button
    wxButton *BtnSetDistanceLimits = new wxButton(panel, ID_BtnSetDistanceLimits, "OK", wxPoint(120, 500), wxSize(80, 26));
    BtnSetDistanceLimits->Raise();
    
    /////////////////////////////////////////////////////////////////////
    // Create StaticBox (GroupBox) on the Mid-right - Reset to Factory //
    /////////////////////////////////////////////////////////////////////
    wxStaticBox *GBReset = new wxStaticBox(panel, ID_GBReset, "Reset to Factory", wxPoint(320, 390), wxSize(268,101));

    //Create a button
    wxButton *BtnReset = new wxButton(panel, ID_BtnReset, "Reset", wxPoint(420, 440), wxSize(80, 26));
    BtnReset->Raise();
    
    // Bind events
    Bind(wxEVT_MENU, &MyForm::OnTFLuna, this, ID_TFLuna);
    Bind(wxEVT_MENU, &MyForm::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyForm::OnExit, this, wxID_EXIT);
    Bind(wxEVT_BUTTON, &MyForm::OnDistanceButton, this, ID_DistanceButton);
    Bind(wxEVT_BUTTON, &MyForm::OnTemperatureButton, this, ID_TemperatureButton);
    Bind(wxEVT_BUTTON, &MyForm::OnAmplitudeButton, this, ID_AmplitudeButton);
    Bind(wxEVT_BUTTON, &MyForm::OnGetRateButton, this, ID_GetRateButton);
    Bind(wxEVT_BUTTON, &MyForm::OnFirmwareButton, this, ID_FirmwareButton);
    Bind(wxEVT_BUTTON, &MyForm::OnSerialNumberButton, this, ID_SerialNumberButton);
    Bind(wxEVT_BUTTON, &MyForm::OnTicksButton, this, ID_TicksButton);
    Bind(wxEVT_BUTTON, &MyForm::OnErrorCodeButton, this, ID_ErrorCodeButton);
    Bind(wxEVT_BUTTON, &MyForm::OnOpCodeButton, this, ID_OpCodeButton);
    Bind(wxEVT_BUTTON, &MyForm::OnPowerModeButton, this, ID_PowerModeButton);
    Bind(wxEVT_BUTTON, &MyForm::OnGetStatusButton, this, ID_GetStatusButton);
    Bind(wxEVT_BUTTON, &MyForm::OnAmpThresholdButton, this, ID_AmpThresholdButton);
    Bind(wxEVT_BUTTON, &MyForm::OnDummyDistButton, this, ID_DummyDistButton);
    Bind(wxEVT_BUTTON, &MyForm::OnMinDistanceButton, this, ID_MinDistanceButton);
    Bind(wxEVT_BUTTON, &MyForm::OnMaxDistanceButton, this, ID_MaxDistanceButton);
    Bind(wxEVT_BUTTON, &MyForm::OnBtnChangeAddress, this, ID_BtnChangeAddress);
    Bind(wxEVT_BUTTON, &MyForm::OnBtnSetI2CAddress, this, ID_BtnSetI2CAddress);
    Bind(wxEVT_BUTTON, &MyForm::OnBtnSetDistanceLimits, this, ID_BtnSetDistanceLimits);
    Bind(wxEVT_BUTTON, &MyForm::OnBtnReset, this, ID_BtnReset);
    
    // Initialize TFLuna - constructor will open I2C connection
    try {
        TFLuna = new MdlTFLuna();
        textArea->AppendText("TF-Luna initialized successfully\n");
    } catch (const std::runtime_error& e) {
        wxMessageBox(wxString::Format("Failed to initialize TF-Luna: %s", e.what()),
                     "Initialization Error", wxOK | wxICON_ERROR);
        TFLuna = nullptr;
    }
}

// Add destructor implementation
MyForm::~MyForm()
{
    // Clean up TFLuna - destructor will close I2C connection
    if (TFLuna != nullptr) {
        delete TFLuna;
        TFLuna = nullptr;
    }
}
 
void MyForm::OnExit(wxCommandEvent& event)
{
    // Destructor will be called automatically when window closes
    Close(true);
}

void MyForm::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a TF-Luna C++ example program",
                 "About TF-Luna", wxOK | wxICON_INFORMATION);
}

void MyForm::OnTFLuna(wxCommandEvent& event)
{
    wxLogMessage("TF-Luna project in C++!");
}

void MyForm::OnDistanceButton(wxCommandEvent& event)
{
    textArea->Clear();
    textArea->SetValue("");
    
    // Check if TFLuna is initialized
    if (TFLuna != nullptr) {
        textArea->AppendText("Distance: ");
        textArea->AppendText(std::to_string(TFLuna->GetDistance()));
        textArea->AppendText(" cm, ");
        float_t Feet = TFLuna->GetDistance() * 0.0328084;
        std::stringstream ss;
        //Format to two decimal points
        ss << std::fixed << std::setprecision(2) << Feet;
        textArea->AppendText(ss.str());
        textArea->AppendText(" Feet \n");
    } else {
        textArea->AppendText("Error: TF-Luna not initialized\n");
    }
}

void MyForm::OnTemperatureButton(wxCommandEvent& event)
{
    // Check if TFLuna is initialized
    if (TFLuna != nullptr) {
        textArea->Clear();
        textArea->AppendText("Temperature: ");
        //Get temperature in Celsius
        float Celsius = TFLuna->GetTemperature();
        std::stringstream ss;
        //Format to two decimal points
        ss << std::fixed << std::setprecision(2) << Celsius;
        textArea->AppendText(ss.str());
        textArea->AppendText(" Celsius \n");
        //Convert to Fahrenheit
        std::stringstream st;
        float Fahrenheit = Celsius * 9/5 + 32;
        st << std::fixed << std::setprecision(2) << Fahrenheit;
        textArea->AppendText(st.str());
        textArea->AppendText(" Fahrenheit \n");
    } else {
        textArea->AppendText("Error: TF-Luna not initialized\n");
    }
}

void MyForm::OnAmplitudeButton(wxCommandEvent& event)
{
    textArea->Clear();
     // Check if TFLuna is initialized
    if (TFLuna != nullptr) {
        textArea->Clear();
        textArea->AppendText("Amplitude: ");
        float Amplitude = TFLuna->GetAmplitude();
        std::stringstream ss;
        //Format to two decimal points
        ss << std::fixed << std::setprecision(2) << Amplitude << "\n";
        textArea->AppendText(ss.str());
        textArea->AppendText("Signal (Between 100 and 65,535 is good). Signal too weak when ");
        textArea->AppendText("below 100. Above 65535 is overexposure, unreliable (object ");
        textArea->AppendText("too close or too reflective). \n");
        
    } else {
        textArea->AppendText("Error: TF-Luna not initialized\n");
    }
}

void MyForm::OnGetRateButton(wxCommandEvent& event)
{
  textArea->Clear();
     // Check if TFLuna is initialized
    if (TFLuna != nullptr) {
        textArea->Clear();
        textArea->AppendText("This TF-Luna reading framerate is: ");
        float FrameRate = TFLuna->GetFrameRate();
        std::stringstream ss;
        //Format to two decimal points
        ss << std::fixed << std::setprecision(2) << FrameRate << " Hz \n";
        textArea->AppendText(ss.str());
        textArea->AppendText("Factory default framerate is 100 Hz");
    } else {
        textArea->AppendText("Error: TF-Luna not initialized\n");
    }
}

void MyForm::OnFirmwareButton(wxCommandEvent& event)
{
    textArea->Clear();
    
    if (TFLuna != nullptr) {
        textArea->AppendText("Firmware version: ");
        std::string version = TFLuna->GetFirmwareVersion();
        textArea->AppendText(wxString(version));
    } else {
        textArea->AppendText("Error: TF-Luna not initialized\n");
    }
}

void MyForm::OnSerialNumberButton(wxCommandEvent& event)
{
    textArea->Clear();
    textArea->AppendText("Serial Number: ");
    std::string SerNo = TFLuna->GetSerialNumber();
    textArea->AppendText(wxString(SerNo));
}

void MyForm::OnTicksButton(wxCommandEvent& event)
{
    textArea->Clear();
    textArea->AppendText("Ticks Value: ");
    uint16_t counter = TFLuna->GetCounterTicks();
    textArea->AppendText(wxString(std::to_string(counter)));
}

void MyForm::OnErrorCodeButton(wxCommandEvent& event)
{
    textArea->Clear();
    textArea->AppendText("Latest Error Code: ");
    uint16_t ErrCode = TFLuna->GetErrorCode();
    textArea->AppendText(wxString(std::to_string(ErrCode)));
    textArea->AppendText("\nHint: Error Status of 0 = no error");
}

void MyForm::OnOpCodeButton(wxCommandEvent& event)
{
    textArea->Clear();
    textArea->AppendText("Operations Mode: ");
    int8_t OpMode = TFLuna->GetOperatingMode();
    textArea->AppendText(wxString(std::to_string(OpMode)));
    textArea->AppendText("\nHint: 0 = Continuous, 1 = Trigger");
}

void MyForm::OnPowerModeButton(wxCommandEvent& event)
{
    textArea->Clear();
    textArea->SetValue("Power Mode: ");
    int8_t PowerMode = TFLuna->GetLowPowerMode();
    textArea->AppendText(wxString(std::to_string(PowerMode)));
    textArea->AppendText("\nHint: 0 = Normal, 1 = Power Saving");
}

void MyForm::OnGetStatusButton(wxCommandEvent& event)
{
    textArea->Clear();
    textArea->SetValue("Enabled Status: ");
    int8_t EnabledStatus = TFLuna->GetEnabledStatus();
    textArea->AppendText(wxString(std::to_string(EnabledStatus)));
    textArea->AppendText("\nHint: 0 = Enabled, 1 = Disabled");
}

void MyForm::OnAmpThresholdButton(wxCommandEvent& event)
{
    textArea->Clear();
    textArea->SetValue("Amplitude Threshold Value: ");
    int8_t AmpThreshold = TFLuna->GetAmplitudeThreshold();
    textArea->AppendText(wxString(std::to_string(AmpThreshold)));
    textArea->AppendText("\nNotes: By Default, when the sensor receives a signal ");
    textArea->AppendText("with a strength less than 100, it will output 0 cm, instead ");
    textArea->AppendText("of trying to calculate the distance.\n");
}

void MyForm::OnDummyDistButton(wxCommandEvent& event)
{
    textArea->Clear();
    textArea->SetValue("Dummy Distance Value: ");
    int16_t DummyDistance = TFLuna->GetDummyDistance();
    textArea->AppendText(wxString(std::to_string(DummyDistance)));
    textArea->AppendText("\nNote: A Distance value that is returned when ");
    textArea->AppendText("the amplitude signal received is too low. ");
    textArea->AppendText("Usually a value of 0.\n");
}

void MyForm::OnMinDistanceButton(wxCommandEvent& event)
{
    textArea->Clear();
    textArea->SetValue("Minimum Distance Value: ");
    int16_t MinDistance = TFLuna->GetMinimumDistance();
    textArea->AppendText(wxString(std::to_string(MinDistance)));
    textArea->AppendText("\nNote: The minimum distance limit is the lowest clamped ");
    textArea->AppendText("value, which is usually the Dummy Distance, or zero. \n");
}

void MyForm::OnMaxDistanceButton(wxCommandEvent& event)
{
    textArea->Clear();
    textArea->SetValue("Maximum Distance Value: ");
    int16_t MaxDistance = TFLuna->GetMaximumDistance();
    textArea->AppendText(wxString(std::to_string(MaxDistance)));
    textArea->AppendText("\nNote: The minimum distance limit is the highest clamped ");
    textArea->AppendText("value, which is usually 800 cm, or 26.2 feet. \n");
}

void MyForm::OnBtnChangeAddress(wxCommandEvent& event)
{
    unsigned long value;
    wxString text = TbAddress->GetValue();
    bool Result = text.ToULong(&value, 0);
    textArea->Clear();
    
    // Check for the value to be between 0 and 255
    if (value >= 0 && value <= 255){
        try {
            uint8_t NewVal = static_cast<uint8_t>(value);
            bool NewAddr = TFLuna->SetI2CAddress(NewVal);
            if (NewAddr == true) {
                textArea->AppendText("Change address successful \n");    
                textArea->AppendText("Please change I2C_ADDR = the new value in MdlTFLunaxx.h ");
                textArea->AppendText("in MdlTFLuna::MdlTFLuna() method, and Reboot the Raspberry Pi. ");
                textArea->AppendText("Test new address in Terminal with 'i2cdetect -y 1' command.\n");
            } else {
                textArea->AppendText("Change address failed \n");    
            }
        } catch (const std::exception& e) {
            textArea->AppendText(wxString::Format("Error: %s\n", e.what()));
        }
    } else {
        textArea->AppendText("Invalid address value. Must be between 0 and 255 (0x00-0xFF).\n");
    }
}
void MyForm::OnBtnSetI2CAddress(wxCommandEvent& event){
    unsigned long value;
    wxString text = TbSetAddress->GetValue();
    bool Result = text.ToULong(&value, 0);
    textArea->Clear();
    
    TFLuna->I2C_ADDR = value;
    textArea->AppendText("Current Temporary I2C Address set to 0x");    
    textArea->AppendText(DecimalToHex(TFLuna->I2C_ADDR));
}

void MyForm::OnBtnSetDistanceLimits(wxCommandEvent& event){
    unsigned long MinVal;
    wxString Min = TbMinDistance->GetValue();
    bool Result = Min.ToULong(&MinVal, 0);
    textArea->Clear();
    bool RetVal = TFLuna->SetMinDistance(MinVal);
    textArea->AppendText("Set Minimum Distance Limit to: ");    
    textArea->AppendText(wxString(std::to_string(MinVal)));
    
    unsigned long MaxVal;
    wxString Max = TbMaxDistance->GetValue();
    Result = Max.ToULong(&MaxVal, 0);
    RetVal = TFLuna->SetMaxDistance(MaxVal);
    textArea->AppendText("Set Maximum Distance Limit to: ");    
    textArea->AppendText(wxString(std::to_string(MaxVal)));
}

void MyForm::OnBtnReset(wxCommandEvent& event) {
    textArea->Clear();
    
    try {
        bool ResVal = TFLuna->Reset();
        textArea->AppendText("Soft Reset TF-Luna to Defaults - Success");
    } catch (const std::exception& e) {
        textArea->AppendText(wxString::Format("Reset Failed: %s", e.what()));
    }
}
