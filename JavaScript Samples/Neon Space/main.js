function main(){
    var SHOW_FPS = false;

    // Three.js init
    var window_extension = 50;
    var scene = new THREE.Scene();
    var camera = new THREE.PerspectiveCamera( 75, (window.outerWidth+window_extension)/(window.outerHeight+window_extension), 1, 5000 );
    camera.position.z = 350;

    var renderer = new THREE.WebGLRenderer();
    renderer.setSize( window.outerWidth+window_extension, window.outerHeight+window_extension);
    document.body.appendChild( renderer.domElement );

    if (SHOW_FPS){
        initFPS(renderer);
    }

    // Scene init
    var geometry = new THREE.CircleGeometry( 24, 24 );
    var geometry2 = new THREE.CircleGeometry( 6, 24 );
    var material = new THREE.MeshBasicMaterial( { color: rgbToHex(0, 0, 0)} );
    var material2 = new THREE.MeshBasicMaterial( { color: rgbToHex(0, 0, 0)} );
    var circle1 = new THREE.Mesh( geometry, material );
    var circle2 = new THREE.Mesh( geometry2, material2 );
    scene.add( circle1);
    scene.add( circle2);
    circle2.material.transparent = true;

    // Position variables
    var x = circle1.position.x,
        y = circle1.position.y,
        velX = 0,
        velY = 0,
        speed = 13.8,
        friction = 0.942;

    // Frame Rate Control variables
    var fps = 72,
        now,
        then = Date.now(),
        interval = 1000/fps,
        delta;

    // Other variables
    var rgb = [0,0,0], i=0;
    var keys = [];

    // Key Events init
    document.body.addEventListener("keydown", function (e) {
        keys[e.keyCode] = true;
    }, false);

    document.body.addEventListener("keyup", function (e) {
        keys[e.keyCode] = false ;
    }, false);

    // Finish init and start update loop
    animate();


    function update() {
        // key updates
        if (keys["W".charCodeAt(0)]){ if (velY < speed) {velY++}} // 87
        if (keys["S".charCodeAt(0)]){ if (velY > -speed) {velY--}} // 83
        if (keys["A".charCodeAt(0)]){ if (velX > -speed) {velX--}} // 65
        if (keys["D".charCodeAt(0)]){ if (velX < speed) {velX++}} // 68

        // position updates
        x += velX;
        y += velY;
        velX *= friction;
        velY *= friction;

        // rgb color updates
        if (i > 360) { i = 0}
        for (var c = 0; c < rgb.length; c++){
            rgb[c] = Math.floor(Math.sin(.4 * i + (2 * c)) * 127 + 128);
        }
        i += 0.20;

        // object updates
        circle2.material.opacity = 1 + Math.sin(0.3  * i);
        circle1.material.color = new THREE.Color(rgbToHex(rgb[0], rgb[1], rgb[2]));
        circle2.material.color = new THREE.Color(rgbToHex(rgb[0], rgb[1], rgb[2]));
        circle1.position.x = x;
        circle1.position.y = y;
        circle2.position.x = x + Math.sin(.6 * i) * 70;
        circle2.position.y = y + Math.cos(.6 * i) * 70;
    }


    function animate() {
        requestAnimationFrame(animate);

        now = Date.now();
        delta = now - then;

        if (delta > interval) {
            then = now - (delta % interval);
            update();
            renderer.render(scene, camera);
            if (SHOW_FPS){ stats.update() }
        }
    }

}

// Start Program
main();


