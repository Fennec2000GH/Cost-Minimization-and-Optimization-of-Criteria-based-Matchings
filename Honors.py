import wx
from wx.lib.platebtn import *
import calendar
import os
import sys
import time
import termcolor


app = wx.App(False, None, True)
window = wx.Frame(None, size = wx.DefaultSize, title = "Window")
panel_main = wx.Panel(window)
label = wx.StaticText(panel_main, label = "Hello World", pos = (0, 0), size = wx.DefaultSize)

#MAIN MENU
btn_exit = wx.Button(panel_main, id = wx.ID_EXIT, pos = wx.DefaultPosition, size = (120, 40))
btn_plate = PlateButton(panel_main, id = wx.ID_DELETE, label = "Magic Button", pos = (0, 60), size = (120, 40), style = PB_STYLE_GRADIENT)
radio_special = wx.RadioButton(panel_main, id = wx.ID_ANY, label = "Special", pos = (200, 20), size = wx.DefaultSize, style = wx.BU_EXACTFIT)
radio_box = wx.RadioBox(panel_main, id = wx.ID_ANY, choices = ["1", "2", "3", "4"], pos = (200, 60), size = wx.DefaultSize, style = wx.RA_SPECIFY_ROWS)
combo_box = wx.ComboBox(panel_main, id = wx.ID_ANY, choices = ["Athena Cai", "Jung Hoesok", "Darren Tay"], pos = (0, 120), size = wx.DefaultSize,
                        style = wx.CB_DROPDOWN | wx.CB_READONLY | wx.CB_SORT)
slider = wx.Slider(panel_main, id = wx.ID_ANY, minValue = 1, maxValue = 10, pos = (0, 160), size = wx.DefaultSize, style = wx.SL_HORIZONTAL | wx.SL_LABELS)

#EVENT HANDLER
#FUNCTION DEFINITIONS
def OnClick(event):
    os.sys.exit()

#BINDINGS
btn_exit.Bind(wx.EVT_BUTTON, OnClick)



#FINALIZE AND RUN
window.Show(True)
app.MainLoop()
