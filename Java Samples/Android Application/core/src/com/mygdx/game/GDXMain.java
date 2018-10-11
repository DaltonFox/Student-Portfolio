package com.mygdx.game;

import com.badlogic.gdx.ApplicationAdapter;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.InputProcessor;
import com.badlogic.gdx.graphics.Color;
import com.badlogic.gdx.graphics.GL20;
import com.badlogic.gdx.graphics.OrthographicCamera;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.BitmapFont;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;
import com.badlogic.gdx.graphics.g2d.freetype.FreeTypeFontGenerator;
import com.badlogic.gdx.graphics.glutils.ShapeRenderer;
import com.badlogic.gdx.math.MathUtils;

import java.util.ArrayList;

public class GDXMain extends ApplicationAdapter implements InputProcessor {
	ShapeRenderer renderer;
	OrthographicCamera camera;
	FreeTypeFontGenerator generator;
	FreeTypeFontGenerator.FreeTypeFontParameter parameter;
	BitmapFont fontLarge;
    BitmapFont fontMedium;
	SpriteBatch uiBatch;
	ArrayList<Place> food_places = new ArrayList<Place>();
	int scroll = 0;
	int ui_buffer = 300;
	int ui_adjuster = 25;
	Color uiColor_dark = new Color(0.3f, 0.3f, 0.3f, 1f);
	Color uiColor_light = new Color(0.5f, 0.5f, 0.5f, 1f);

	@Override
	public void create () {
	    renderer = new ShapeRenderer();

	    uiBatch = new SpriteBatch();

        camera = new OrthographicCamera(Gdx.graphics.getWidth(), Gdx.graphics.getHeight());
        camera.setToOrtho(true, Gdx.graphics.getWidth(), Gdx.graphics.getHeight());
        camera.update();

        for (int i = 0; i < 12; i++)
        {
            String index = Integer.toString(i + 1);
            boolean open = i % 2 == 0;
            food_places.add(new Place("Test Food Place", "Test Address 940 2nd Street,  " + index + " miles away", open));
        }


        generator = new FreeTypeFontGenerator(Gdx.files.internal("uiFont.ttf"));
        parameter = new FreeTypeFontGenerator.FreeTypeFontParameter();
        parameter.size = 118;
        fontLarge = generator.generateFont(parameter);
        parameter.size = 64;
        fontMedium = generator.generateFont(parameter);

        Gdx.input.setInputProcessor(this);
	}

	@Override
	public void render () {
        Gdx.gl.glClearColor(0.1f, 0.1f, 0.1f, 1);
        Gdx.gl.glClear(GL20.GL_COLOR_BUFFER_BIT);


	    camera.update();
        renderer.setProjectionMatrix(camera.combined);

        renderer.begin(ShapeRenderer.ShapeType.Filled);
        renderer.setColor(uiColor_light);
        for (int i = 0; i < food_places.size(); i++)
        {
            int spot = (ui_buffer * (i + 1) - scroll);
            renderer.rect(0, spot - ui_adjuster, Gdx.graphics.getWidth(), ui_buffer - (ui_adjuster * 2));
        }
        renderer.end();

        Gdx.gl.glLineWidth(16);
        renderer.begin(ShapeRenderer.ShapeType.Line);
        renderer.setColor(Color.ROYAL);
        for (int i = 0; i < food_places.size(); i++)
        {
            int spot = (ui_buffer * (i + 1) - scroll);
            renderer.rect(0, spot - ui_adjuster, Gdx.graphics.getWidth(), ui_buffer - (ui_adjuster * 2));
        }
        renderer.end();

        uiBatch.begin();
        fontLarge.setColor(Color.WHITE);
        for (int i = 0; i < food_places.size(); i++)
        {
            int spot = scroll + Gdx.graphics.getHeight() - (ui_buffer * (i + 1));
            fontLarge.draw(uiBatch, food_places.get(i).name, ui_adjuster, spot);
            fontMedium.draw(uiBatch, "Open: " + Boolean.toString(food_places.get(i).open), (int) (Gdx.graphics.getWidth() -  (ui_buffer * 1.30)), spot - ui_adjuster);
            fontMedium.draw(uiBatch, food_places.get(i).address, ui_adjuster * 4, spot - (ui_buffer / 2));
        }
        uiBatch.end();

        renderer.begin(ShapeRenderer.ShapeType.Filled);
        renderer.setColor(uiColor_dark);
        renderer.rect(0, 0, Gdx.graphics.getWidth(), ui_buffer - (ui_adjuster * 3));
        renderer.end();

        Gdx.gl.glLineWidth(20);
        renderer.begin(ShapeRenderer.ShapeType.Line);
        renderer.setColor(Color.BLACK);
        renderer.rect(0, 0, Gdx.graphics.getWidth(), ui_buffer - (ui_adjuster * 3));
        renderer.end();

        uiBatch.begin();
        fontMedium.setColor(Color.WHITE);
        fontMedium.draw(uiBatch, "Nearby Place Finder Application", ui_adjuster, (int) (Gdx.graphics.getHeight() - ui_adjuster * 1.5));
        fontMedium.draw(uiBatch, "Searching for all food places within 12 miles", ui_adjuster, Gdx.graphics.getHeight() - (ui_buffer / 2));
        uiBatch.end();
	}
	
	@Override
	public void dispose () {
        renderer.dispose();
        generator.dispose();
        fontLarge.dispose();
        fontMedium.dispose();
	}

    @Override
    public boolean keyDown(int keycode) {
        return false;
    }

    @Override
    public boolean keyUp(int keycode) {
        return false;
    }

    @Override
    public boolean keyTyped(char character) {
        return false;
    }

    @Override
    public boolean touchDown(int screenX, int screenY, int pointer, int button) {
        return false;
    }

    @Override
    public boolean touchUp(int screenX, int screenY, int pointer, int button) {
        return false;
    }

    @Override
    public boolean touchDragged(int screenX, int screenY, int pointer) {
	    scroll -= Gdx.input.getDeltaY();
        System.out.println(scroll + " " + ui_buffer * (food_places.size() - (Gdx.graphics.getHeight() / ui_buffer)));
	    // Bounds
	    if (scroll < 0)
            scroll = 0;
        if (scroll > ui_buffer * (food_places.size() - (Gdx.graphics.getHeight() / ui_buffer)))
            scroll = ui_buffer * (food_places.size() - (Gdx.graphics.getHeight() / ui_buffer));
        return false;
    }

    @Override
    public boolean mouseMoved(int screenX, int screenY) {
        return false;
    }

    @Override
    public boolean scrolled(int amount) {
        return false;
    }
}
