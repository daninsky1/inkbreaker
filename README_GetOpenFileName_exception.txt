Fl_Native_File_Chooser is throwing access violation, I need to see if there a
problem with the FLTK GetOpenFileName configuration, or is the problem with
GetOpenFileName itself.

Update: GetOpenFileName create new folder and the menu shell throw an exception
in the same place, I think that some explorer plugin or add-on is intefering in
it's behavior. I need to set up a new Windows to do the test since that is no
way, that I know, to disable explorer plugins and add-ons. Maybe use the window
container if that is possible.

Update: I deed the test with a fresh windows and inded has no problems, so now
I need to investigate wich add-on is causing the problem.

Update: The problem was caused by the catalist control center explorer add-on
that loads when you right click or use a shortcut. I'm not current using amd
graphics card so I uninstall and know works fine. If I install it again I need
to test if it is possible to disable the exprorer add-on.

