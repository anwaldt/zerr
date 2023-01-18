p = Server.default.options;

p.numInputBusChannels = 16;

s.boot;

ServerMeter(s);

FreqScope.new(400, 200, 2, server: s);

{ SinOsc.ar(2000, 0, 0.25) }.play(s);


