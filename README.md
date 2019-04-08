#geany-qop-plugin

Geany Quick Open File Plugin

<p class="topic-title first">
Contents
</p>
<ul class="simple">
<li>
<a class="reference internal" href="#about" id="id1" name="id1">About</a>
</li>
<li>
<a class="reference internal" href="#installation" id="id1" name="id1">Installation</a>
</li>
<li>
<a class="reference internal" href="#usage" id="id2" name="id2">Usage</a>
</li>
<li>
<a class="reference internal" href="#requirements" id="id3" name="id3">Requirements</a>
</li>
<li>
<a class="reference internal" href="#contact-developers" id="id4" name="id4">Contact developers</a>
</li>
</ul>
</div>
<div class="section" id="about">
<h3>
About
</h3>
<p>
Quick Open File offers you the possibility to reach quickly a file. Without using your mouse. Instead you set a keybinding to quicken the task.
</p>
</div>
<div class="section" id="installation">

<h3>
Installation
</h3>

<p>Download the plugin :  https://github.com/samyBadjoudj/geany-qop-plugin/blob/master/qop.c
</p>
<p>
git clone git@github.com:samyBadjoudj/geany-qop-plugin.git
</p>
<p>cd geany-qop-plugin</p>
<p>


Run : <br/>
	sudo gcc -c  qop.c `pkg-config --cflags --libs  geany`  -fPIC;gcc -shared -o qop.so qop.o;<br/>
	sudo cp ./qop.so /file/of/geany/plugins/ (ex:/usr/lib/x86_64-linux-gnu/geany)
</p>

</div>
<div class="section" id="usage">
<h3>
Usage
</h3>
<p>
After installed successfully, load the plugin in Geany's plugin manager.
Go to Tools and click on "Quick Open File".
Here a POP-UP will help you to find your file quickly, by typing some letters that are in the name.
Then select the file by the arrows (up or down), type enter... or esc to dismiss the POP-UP
<img src="http://s1.postimg.org/69i6bssof/qop_geany.png" alt="Geany Quick Open File Plugin" />
</p>

</div>
<div class="section" id="requirements">
<h3>
Requirements
</h3>
<ul class="simple">
<li>GTK &gt;= 2.8.0
</li>
</ul>
</div>
<div class="section" id="contact-developers">
<h3>
Contact developers
</h3>
<p>
Samy Badjoudj &lt;samy(dot)badjoudj(at)gmail(dot)com&gt;
</p>
</div>
