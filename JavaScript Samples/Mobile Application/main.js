var container;
var camera, scene, renderer, particles, geometry, materials = [], opacities = [], i, size;
var windowHalfX = window.innerWidth / 2;
var windowHalfY = window.innerHeight / 2;
var mouseX = 0, mouseY = 0;
var first_load = true;
var scene_density = 30;
var liElements = document.getElementsByTagName("li");
var h1Elements = document.getElementsByTagName("h1");
var textDivs = document.getElementsByClassName("text-div");
var pElements = document.getElementsByClassName("menu_li");
var tElements = document.getElementsByTagName("td");
var hbuffer = 1520;
var mobile_device = false;
var rgb = [0,0,0], r=0;

init();
animate();

function init()
{
    // Scene initialization
    container = document.createElement( 'div' );
    document.body.appendChild( container );
    camera = new THREE.PerspectiveCamera( 75, window.innerWidth / window.innerHeight, 1, 2500 );
    camera.position.z = 800;
    scene = new THREE.Scene();
    scene.fog = new THREE.FogExp2( 0xffffff, 0.00025 );
    geometry = new THREE.Geometry();
    renderer = new THREE.WebGLRenderer( {alpha : true});
    renderer.setPixelRatio( window.devicePixelRatio );
    renderer.setSize( window.innerWidth, window.innerHeight );
    container.appendChild( renderer.domElement );

    // Particle texture initialization
    var textureLoader = new THREE.TextureLoader();
    var particle_sprite = textureLoader.load( "Assets/ava_particle.png" );

    // Generate set of points of density value
    for ( i = 0; i < scene_density; i ++ )
    {
        var vertex = new THREE.Vector3();
        vertex.x = Math.random() * 1500 - 750;
        vertex.y = Math.random() * 1500 - 750;
        vertex.z = Math.random() * 1500 - 750;
        geometry.vertices.push( vertex );
    }

    // Populate generated points at x number of sets of x size
    var size_sets = [60, 40, 25];
    for ( i = 0; i < 3; i ++ )
    {
        size = size_sets[i];
        materials[i] =
        new THREE.PointsMaterial
        ({
            size: size, map: particle_sprite, blending: THREE.AdditiveBlending, depthTest: false, transparent : true
        });
        materials[i].color.setRGB(0.9, 0.05, 0.6);
        materials[i].opacity = Math.random() + 0.7;
        if (materials[i].opacity >= 1)
        {
            materials[i].opacity = 0.98
        }
        opacities[i] = materials[i].opacity;
        materials[i].opacity = 0;
        particles = new THREE.Points( geometry, materials[i] );
        particles.rotation.x = Math.random() * 9;
        particles.rotation.y = Math.random() * 9;
        particles.rotation.z = Math.random() * 9;
        scene.add( particles );
    }

    // Run mobile detection specific code
    mobile_device = window.mobilecheck();
    if (mobile_device === false)
    {
        window.addEventListener( 'resize', onWindowResize, false );
        document.addEventListener( 'mousemove', onDocumentMouseMove, false );
        onWindowResize();
        document.getElementById("sub-menu").style.paddingTop = "5vh";
        var dElements = document.getElementsByClassName("footer_div");
        for(var i = 0; i < dElements.length; i++)
        {
            dElements[i].style.height = "55px";
        }
    }

    // Sub Menu Controls
    var sub_menu_open = false;
    var pull_up_open = false;
    window.onload = function()
    {
        document.getElementById("up-button").onclick = function up()
        {
            var trans = new TWEEN.Tween( document.getElementById("main-transition").style ).to( { opacity: 0.1}, 100).start();
            trans.onComplete(function() {
                document.getElementById("main-transition").style.opacity = 0;
                window.scrollTo(0, 0);
            });

        };
        document.getElementById("nav-button").onclick = function sub_menu_control()
        {
            if (sub_menu_open === false)
            {
                document.getElementById("sub-menu").style.opacity = 1;
                document.getElementById("sub-menu").style.left = "0vw";
                document.getElementById("sub-menu").style.top = "0vh";
                document.getElementById("nav-button").src = "Assets/exit.png";
                document.body.style.overflow = "hidden";
                sub_menu_open = true
            }
            else if (sub_menu_open === true)
            {
                document.getElementById("sub-menu").style.opacity = 0;
                document.getElementById("sub-menu").style.top = "-3vh";
                document.getElementById("sub-menu").style.left = "100vw";
                document.getElementById("nav-button").src = "Assets/drop.png";
                document.body.style.overflow = "scroll";
                sub_menu_open = false
            }
            if (pull_up_open)
            {
                document.getElementById("pull-up").style.opacity = 0;
                document.getElementById("pull-up").style.top = "-3vw";
                document.getElementById("pull-up").style.left = "100vw";
                pull_up_open = false
            }
        };
        document.getElementById("footer-terms").onclick = function sub_menu_terms()
        {
            document.getElementById("pull-up").scrollTop = 0;
            document.getElementById("pull-up").style.opacity = 1;
            document.getElementById("pull-up-head").textContent = "Terms of Service";
            document.getElementById("pull-up-text").textContent = window.termsofservice;
            document.getElementById("pull-up").style.left = "0vw";
            document.getElementById("pull-up").style.top = "0vh";
            document.getElementById("nav-button").src = "Assets/exit.png";
            document.body.style.overflow = "hidden";
            sub_menu_open = true;
            pull_up_open = true;
        };
        document.getElementById("footer-policy").onclick = function sub_menu_policy()
        {
            document.getElementById("pull-up").scrollTop = 0;
            document.getElementById("pull-up").style.opacity = 1;
            document.getElementById("pull-up-head").textContent = "Privacy Policy";
            document.getElementById("pull-up-text").textContent = window.privacypolicy;
            document.getElementById("pull-up").style.left = "0vw";
            document.getElementById("pull-up").style.top = "0vh";
            document.getElementById("nav-button").src = "Assets/exit.png";
            document.body.style.overflow = "hidden";
            sub_menu_open = true;
            pull_up_open = true;
        }
    }
}

function onDocumentMouseMove( event )
{
    mouseX = event.clientX - windowHalfX;
    mouseY = event.clientY - windowHalfY;
}

function onWindowResize()
{
    // Resize renderer properties to fit new window size
    windowHalfX = window.innerWidth / 2;
    windowHalfY = window.innerHeight / 2;
    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();
    renderer.setSize( window.innerWidth, window.innerHeight );

    // PC Scaling Code
    var uni_scale = (1 + (parseFloat(window.innerWidth) / parseFloat(window.innerHeight)));

    if (window.innerHeight > 1080)
    {
        uni_scale += window.innerHeight / 1000
    }
    if (window.innerHeight > 1440)
    {
        uni_scale += window.innerHeight / 5000
    }
    var h_scale = 24 + Math.pow(uni_scale, uni_scale / 2.45);
    if (h_scale > 89){
        h_scale = 89
    }
    for(var i = 0; i < h1Elements.length; i++)
    {
        h1Elements[i].style.fontSize = h_scale.toString() + "px";
    }
    for(var i = 0; i < tElements.length; i++)
    {
        tElements[i].style.fontSize = (h_scale/2.25).toString() + "px";
    }

    for(var i = 0; i < textDivs.length; i++)
    {
        if (window.innerWidth > hbuffer)
        {
            textDivs[i].style.position = "absolute";
            textDivs[i].style.left = ((window.innerWidth - hbuffer) / 2 + 100).toString() + "px";
        }
        else
        {
            textDivs[i].style.left = (100).toString() + "px";
        }
    }

    var l_scale = 18 + Math.pow(uni_scale, uni_scale / 2.90);
    if (l_scale > 39){
        l_scale = 39
    }
    var width_scale = (hbuffer - 500) * (90 - (uni_scale * 3));
    if (width_scale > hbuffer - 200)
    {
        width_scale = hbuffer - 200;
    }
    for(var i = 0; i < liElements.length; i++)
    {
        liElements[i].style.fontSize = l_scale.toString() + "px";
        liElements[i].style.maxWidth = width_scale.toString() + "px";
    }

    document.getElementById("copy-right").style.fontSize = (l_scale * 0.45).toString() + "px";

    if (window.innerWidth > hbuffer)
    {
        document.getElementById("nav-logo").style.left = ((window.innerWidth - hbuffer) / 2 + 50).toString() + "px";
        document.getElementById("nav-button").style.left = (window.innerWidth - ((window.innerWidth - hbuffer)/2) - 120).toString() + "px";
        document.getElementById("up-button").style.left = (window.innerWidth - ((window.innerWidth - hbuffer)/2) - 135).toString() + "px";
        document.getElementById("left-buffer").style.width = ((window.innerWidth - hbuffer) / 2).toString() + "px";
        document.getElementById("right-buffer").style.width = ((window.innerWidth - hbuffer) / 2).toString() + "px";
        document.getElementById("right-buffer").style.left = (window.innerWidth - ((window.innerWidth - hbuffer)/2).toString() + "px");
    }
    else
    {
        document.getElementById("nav-logo").style.left = (50).toString() + "px";
        document.getElementById("nav-button").style.left = (window.innerWidth - 100).toString() + "px";
        document.getElementById("up-button").style.left = (window.innerWidth - 115).toString() + "px";
        document.getElementById("left-buffer").style.width = 0;
        document.getElementById("right-buffer").style.width = 0;
        document.getElementById("right-buffer").style.left = "100vw";
    }

    document.getElementById("main-quoter").style.fontSize = (l_scale * 1.55).toString() + "px";

    for(var i = 0; i < pElements.length; i++)
    {
        if (window.innerWidth > hbuffer){
            pElements[i].style.left = (((window.innerWidth - hbuffer)/2) + 70).toString() + "px";
            pElements[i].style.width = (hbuffer - 150).toString() + "px";
            pElements[i].style.fontSize = (l_scale / 1.15).toString() + "px";
        }
        else
        {
            pElements[i].style.left = (70).toString() + "px";
            pElements[i].style.width = ((window.innerWidth - 150)).toString() + "px";
            pElements[i].style.fontSize = (l_scale / 1.15).toString() + "px";
        }
    }
}

function animate()
{
    // Run animation loop
    render();
    requestAnimationFrame( animate );
}

function render()
{
    TWEEN.update();
    // if (r > 360) { r = 0}
    // for (var c = 0; c < rgb.length; c++){
    //     rgb[c] = Math.floor(Math.sin(.4 * r + (2 * c)) * 127 + 128);
    // }
    // r += 0.07;
    
    // Movement Update
    var time = Date.now() * 0.0000722;

    // PC Mouse Input
    if (mobile_device === false)
    {
        camera.position.x += ( mouseX - camera.position.x ) * 0.08;
        camera.position.y += ( - mouseY - camera.position.y ) * 0.08;
        camera.lookAt( scene.position );
    }

    // Ava Visual Update
    for ( i = 0; i < scene.children.length; i ++ )
    {
        var object = scene.children[ i ];
        //object.material.color.setRGB(rgb[0] / 255, rgb[1] / 255, rgb[2] / 255);
        if ( object instanceof THREE.Points )
        {
            object.rotation.y = time * ( i < 4 ? i + 2 : - ( i + 2 ) );
            if (first_load === true)
            {
                new TWEEN.Tween( object.material ).to( { opacity: opacities[i]}, 500).start();
            }
        }
    }
    if (first_load === true)
    {
        first_load = false;
    }

    renderer.render( scene, camera );
}