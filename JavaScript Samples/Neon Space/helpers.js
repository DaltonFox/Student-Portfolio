function rgbToHex(r, g, b) {
    return "#" + ((1 << 24) + (r << 16) + (g << 8) + b).toString(16).slice(1);
}

function initFPS(renderer){
    var container;
    container = document.createElement( 'div' );
    document.body.appendChild( container );
    container.appendChild( renderer.domElement );
    stats = new Stats();
    container.appendChild( stats.dom );
}