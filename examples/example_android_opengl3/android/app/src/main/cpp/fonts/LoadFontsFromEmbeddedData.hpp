#include <cstring> // For strncpy
    
// Function to load fonts from the embedded compressed data
void LoadFontsFromEmbeddedData()
{
    ImFontConfig font_config;

    // Set oversampling for better quality
    font_config.OversampleH = 4;  // Higher value means better horizontal text quality
    font_config.OversampleV = 4;  // Higher value means better vertical text quality
    font_config.PixelSnapH = true; // Align pixels to the grid for sharper text

    // Afacadflux-Black Fonts
    strncpy(font_config.Name, "Afacadflux-Black", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(AfacadFlux_Black_compressed_data, AfacadFlux_Black_compressed_size, 23.0f, &font_config);

    // Afacadflux-Bold Fonts
    strncpy(font_config.Name, "Afacadflux-Bold", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(AfacadFlux_Bold_compressed_data, AfacadFlux_Bold_compressed_size, 23.0f, &font_config);

    // Afacadflux-Extrabold Fonts
    strncpy(font_config.Name, "Afacadflux-Extrabold", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(AfacadFlux_ExtraBold_compressed_data, AfacadFlux_ExtraBold_compressed_size, 23.0f, &font_config);

    // Afacadflux-Extralight Fonts
    strncpy(font_config.Name, "Afacadflux-Extralight", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(AfacadFlux_ExtraLight_compressed_data, AfacadFlux_ExtraLight_compressed_size, 23.0f, &font_config);

    // Afacadflux-Light Fonts
    strncpy(font_config.Name, "Afacadflux-Light", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(AfacadFlux_Light_compressed_data, AfacadFlux_Light_compressed_size, 23.0f, &font_config);

    // Afacadflux-Medium Fonts
    strncpy(font_config.Name, "Afacadflux-Medium", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(AfacadFlux_Medium_compressed_data, AfacadFlux_Medium_compressed_size, 23.0f, &font_config);

    // Afacadflux-Regular Fonts
    strncpy(font_config.Name, "Afacadflux-Regular", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(AfacadFlux_Regular_compressed_data, AfacadFlux_Regular_compressed_size, 23.0f, &font_config);

    // Afacadflux-Semibold Fonts
    strncpy(font_config.Name, "Afacadflux-Semibold", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(AfacadFlux_SemiBold_compressed_data, AfacadFlux_SemiBold_compressed_size, 23.0f, &font_config);

    // Afacadflux-Thin Fonts
    strncpy(font_config.Name, "Afacadflux-Thin", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(AfacadFlux_Thin_compressed_data, AfacadFlux_Thin_compressed_size, 23.0f, &font_config);

    // Montserrat-Black Fonts
    strncpy(font_config.Name, "Montserrat-Black", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Montserrat_Black_compressed_data, Montserrat_Black_compressed_size, 23.0f, &font_config);

    // Montserrat-Blackitalic Fonts
    strncpy(font_config.Name, "Montserrat-Blackitalic", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Montserrat_BlackItalic_compressed_data, Montserrat_BlackItalic_compressed_size, 23.0f, &font_config);

    // Montserrat-Bold Fonts
    strncpy(font_config.Name, "Montserrat-Bold", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Montserrat_Bold_compressed_data, Montserrat_Bold_compressed_size, 23.0f, &font_config);

    // Montserrat-Bolditalic Fonts
    strncpy(font_config.Name, "Montserrat-Bolditalic", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Montserrat_BoldItalic_compressed_data, Montserrat_BoldItalic_compressed_size, 23.0f, &font_config);

    // Montserrat-Extrabold Fonts
    strncpy(font_config.Name, "Montserrat-Extrabold", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Montserrat_ExtraBold_compressed_data, Montserrat_ExtraBold_compressed_size, 23.0f, &font_config);

    // Montserrat-Extrabolditalic Fonts
    strncpy(font_config.Name, "Montserrat-Extrabolditalic", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Montserrat_ExtraBoldItalic_compressed_data, Montserrat_ExtraBoldItalic_compressed_size, 23.0f, &font_config);

    // Montserrat-Extralight Fonts
    strncpy(font_config.Name, "Montserrat-Extralight", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Montserrat_ExtraLight_compressed_data, Montserrat_ExtraLight_compressed_size, 23.0f, &font_config);

    // Montserrat-Extralightitalic Fonts
    strncpy(font_config.Name, "Montserrat-Extralightitalic", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Montserrat_ExtraLightItalic_compressed_data, Montserrat_ExtraLightItalic_compressed_size, 23.0f, &font_config);

    // Montserrat-Italic Fonts
    strncpy(font_config.Name, "Montserrat-Italic", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Montserrat_Italic_compressed_data, Montserrat_Italic_compressed_size, 23.0f, &font_config);

    // Montserrat-Light Fonts
    strncpy(font_config.Name, "Montserrat-Light", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Montserrat_Light_compressed_data, Montserrat_Light_compressed_size, 23.0f, &font_config);

    // Montserrat-Lightitalic Fonts
    strncpy(font_config.Name, "Montserrat-Lightitalic", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Montserrat_LightItalic_compressed_data, Montserrat_LightItalic_compressed_size, 23.0f, &font_config);

    // Montserrat-Medium Fonts
    strncpy(font_config.Name, "Montserrat-Medium", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Montserrat_Medium_compressed_data, Montserrat_Medium_compressed_size, 23.0f, &font_config);

    // Montserrat-Mediumitalic Fonts
    strncpy(font_config.Name, "Montserrat-Mediumitalic", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Montserrat_MediumItalic_compressed_data, Montserrat_MediumItalic_compressed_size, 23.0f, &font_config);

    // Montserrat-Regular Fonts
    strncpy(font_config.Name, "Montserrat-Regular", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Montserrat_Regular_compressed_data, Montserrat_Regular_compressed_size, 23.0f, &font_config);

    // Montserrat-Semibold Fonts
    strncpy(font_config.Name, "Montserrat-Semibold", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Montserrat_SemiBold_compressed_data, Montserrat_SemiBold_compressed_size, 23.0f, &font_config);

    // Montserrat-Semibolditalic Fonts
    strncpy(font_config.Name, "Montserrat-Semibolditalic", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Montserrat_SemiBoldItalic_compressed_data, Montserrat_SemiBoldItalic_compressed_size, 23.0f, &font_config);

    // Montserrat-Thin Fonts
    strncpy(font_config.Name, "Montserrat-Thin", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Montserrat_Thin_compressed_data, Montserrat_Thin_compressed_size, 23.0f, &font_config);

    // Montserrat-Thinitalic Fonts
    strncpy(font_config.Name, "Montserrat-Thinitalic", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Montserrat_ThinItalic_compressed_data, Montserrat_ThinItalic_compressed_size, 23.0f, &font_config);

    // Oswald-Bold Fonts
    strncpy(font_config.Name, "Oswald-Bold", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Oswald_Bold_compressed_data, Oswald_Bold_compressed_size, 23.0f, &font_config);

    // Oswald-Extralight Fonts
    strncpy(font_config.Name, "Oswald-Extralight", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Oswald_ExtraLight_compressed_data, Oswald_ExtraLight_compressed_size, 23.0f, &font_config);

    // Oswald-Light Fonts
    strncpy(font_config.Name, "Oswald-Light", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Oswald_Light_compressed_data, Oswald_Light_compressed_size, 23.0f, &font_config);

    // Oswald-Medium Fonts
    strncpy(font_config.Name, "Oswald-Medium", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Oswald_Medium_compressed_data, Oswald_Medium_compressed_size, 23.0f, &font_config);

    // Oswald-Regular Fonts
    strncpy(font_config.Name, "Oswald-Regular", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Oswald_Regular_compressed_data, Oswald_Regular_compressed_size, 23.0f, &font_config);

    // Oswald-Semibold Fonts
    strncpy(font_config.Name, "Oswald-Semibold", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Oswald_SemiBold_compressed_data, Oswald_SemiBold_compressed_size, 23.0f, &font_config);

    // Playfairdisplay-Black Fonts
    strncpy(font_config.Name, "Playfairdisplay-Black", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(PlayfairDisplay_Black_compressed_data, PlayfairDisplay_Black_compressed_size, 23.0f, &font_config);

    // Playfairdisplay-Blackitalic Fonts
    strncpy(font_config.Name, "Playfairdisplay-Blackitalic", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(PlayfairDisplay_BlackItalic_compressed_data, PlayfairDisplay_BlackItalic_compressed_size, 23.0f, &font_config);

    // Playfairdisplay-Bold Fonts
    strncpy(font_config.Name, "Playfairdisplay-Bold", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(PlayfairDisplay_Bold_compressed_data, PlayfairDisplay_Bold_compressed_size, 23.0f, &font_config);

    // Playfairdisplay-Bolditalic Fonts
    strncpy(font_config.Name, "Playfairdisplay-Bolditalic", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(PlayfairDisplay_BoldItalic_compressed_data, PlayfairDisplay_BoldItalic_compressed_size, 23.0f, &font_config);

    // Playfairdisplay-Extrabold Fonts
    strncpy(font_config.Name, "Playfairdisplay-Extrabold", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(PlayfairDisplay_ExtraBold_compressed_data, PlayfairDisplay_ExtraBold_compressed_size, 23.0f, &font_config);

    // Playfairdisplay-Extrabolditalic Fonts
    strncpy(font_config.Name, "Playfairdisplay-Extrabolditalic", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(PlayfairDisplay_ExtraBoldItalic_compressed_data, PlayfairDisplay_ExtraBoldItalic_compressed_size, 23.0f, &font_config);

    // Playfairdisplay-Italic Fonts
    strncpy(font_config.Name, "Playfairdisplay-Italic", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(PlayfairDisplay_Italic_compressed_data, PlayfairDisplay_Italic_compressed_size, 23.0f, &font_config);

    // Playfairdisplay-Medium Fonts
    strncpy(font_config.Name, "Playfairdisplay-Medium", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(PlayfairDisplay_Medium_compressed_data, PlayfairDisplay_Medium_compressed_size, 23.0f, &font_config);

    // Playfairdisplay-Mediumitalic Fonts
    strncpy(font_config.Name, "Playfairdisplay-Mediumitalic", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(PlayfairDisplay_MediumItalic_compressed_data, PlayfairDisplay_MediumItalic_compressed_size, 23.0f, &font_config);

    // Playfairdisplay-Regular Fonts
    strncpy(font_config.Name, "Playfairdisplay-Regular", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(PlayfairDisplay_Regular_compressed_data, PlayfairDisplay_Regular_compressed_size, 23.0f, &font_config);

    // Playfairdisplay-Semibold Fonts
    strncpy(font_config.Name, "Playfairdisplay-Semibold", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(PlayfairDisplay_SemiBold_compressed_data, PlayfairDisplay_SemiBold_compressed_size, 23.0f, &font_config);

    // Playfairdisplay-Semibolditalic Fonts
    strncpy(font_config.Name, "Playfairdisplay-Semibolditalic", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(PlayfairDisplay_SemiBoldItalic_compressed_data, PlayfairDisplay_SemiBoldItalic_compressed_size, 23.0f, &font_config);

    // Robotocondensed-Black Fonts
    strncpy(font_config.Name, "Robotocondensed-Black", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoCondensed_Black_compressed_data, RobotoCondensed_Black_compressed_size, 23.0f, &font_config);

    // Robotocondensed-Blackitalic Fonts
    strncpy(font_config.Name, "Robotocondensed-Blackitalic", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoCondensed_BlackItalic_compressed_data, RobotoCondensed_BlackItalic_compressed_size, 23.0f, &font_config);

    // Robotocondensed-Bold Fonts
    strncpy(font_config.Name, "Robotocondensed-Bold", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoCondensed_Bold_compressed_data, RobotoCondensed_Bold_compressed_size, 23.0f, &font_config);

    // Robotocondensed-Bolditalic Fonts
    strncpy(font_config.Name, "Robotocondensed-Bolditalic", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoCondensed_BoldItalic_compressed_data, RobotoCondensed_BoldItalic_compressed_size, 23.0f, &font_config);

    // Robotocondensed-Extrabold Fonts
    strncpy(font_config.Name, "Robotocondensed-Extrabold", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoCondensed_ExtraBold_compressed_data, RobotoCondensed_ExtraBold_compressed_size, 23.0f, &font_config);

    // Robotocondensed-Extrabolditalic Fonts
    strncpy(font_config.Name, "Robotocondensed-Extrabolditalic", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoCondensed_ExtraBoldItalic_compressed_data, RobotoCondensed_ExtraBoldItalic_compressed_size, 23.0f, &font_config);

    // Robotocondensed-Extralight Fonts
    strncpy(font_config.Name, "Robotocondensed-Extralight", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoCondensed_ExtraLight_compressed_data, RobotoCondensed_ExtraLight_compressed_size, 23.0f, &font_config);

    // Robotocondensed-Extralightitalic Fonts
    strncpy(font_config.Name, "Robotocondensed-Extralightitalic", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoCondensed_ExtraLightItalic_compressed_data, RobotoCondensed_ExtraLightItalic_compressed_size, 23.0f, &font_config);

    // Robotocondensed-Italic Fonts
    strncpy(font_config.Name, "Robotocondensed-Italic", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoCondensed_Italic_compressed_data, RobotoCondensed_Italic_compressed_size, 23.0f, &font_config);

    // Robotocondensed-Light Fonts
    strncpy(font_config.Name, "Robotocondensed-Light", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoCondensed_Light_compressed_data, RobotoCondensed_Light_compressed_size, 23.0f, &font_config);

    // Robotocondensed-Lightitalic Fonts
    strncpy(font_config.Name, "Robotocondensed-Lightitalic", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoCondensed_LightItalic_compressed_data, RobotoCondensed_LightItalic_compressed_size, 23.0f, &font_config);

    // Robotocondensed-Medium Fonts
    strncpy(font_config.Name, "Robotocondensed-Medium", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoCondensed_Medium_compressed_data, RobotoCondensed_Medium_compressed_size, 23.0f, &font_config);

    // Robotocondensed-Mediumitalic Fonts
    strncpy(font_config.Name, "Robotocondensed-Mediumitalic", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoCondensed_MediumItalic_compressed_data, RobotoCondensed_MediumItalic_compressed_size, 23.0f, &font_config);

    // Robotocondensed-Regular Fonts
    strncpy(font_config.Name, "Robotocondensed-Regular", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoCondensed_Regular_compressed_data, RobotoCondensed_Regular_compressed_size, 23.0f, &font_config);

    // Robotocondensed-Semibold Fonts
    strncpy(font_config.Name, "Robotocondensed-Semibold", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoCondensed_SemiBold_compressed_data, RobotoCondensed_SemiBold_compressed_size, 23.0f, &font_config);

    // Robotocondensed-Semibolditalic Fonts
    strncpy(font_config.Name, "Robotocondensed-Semibolditalic", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoCondensed_SemiBoldItalic_compressed_data, RobotoCondensed_SemiBoldItalic_compressed_size, 23.0f, &font_config);

    // Robotocondensed-Thin Fonts
    strncpy(font_config.Name, "Robotocondensed-Thin", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoCondensed_Thin_compressed_data, RobotoCondensed_Thin_compressed_size, 23.0f, &font_config);

    // Robotocondensed-Thinitalic Fonts
    strncpy(font_config.Name, "Robotocondensed-Thinitalic", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoCondensed_ThinItalic_compressed_data, RobotoCondensed_ThinItalic_compressed_size, 23.0f, &font_config);

    // Robotoflex-Regular Fonts
    strncpy(font_config.Name, "Robotoflex-Regular", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoFlex_Regular_compressed_data, RobotoFlex_Regular_compressed_size, 23.0f, &font_config);

    // Robotomono-Bold Fonts
    strncpy(font_config.Name, "Robotomono-Bold", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoMono_Bold_compressed_data, RobotoMono_Bold_compressed_size, 23.0f, &font_config);

    // Robotomono-Bolditalic Fonts
    strncpy(font_config.Name, "Robotomono-Bolditalic", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoMono_BoldItalic_compressed_data, RobotoMono_BoldItalic_compressed_size, 23.0f, &font_config);

    // Robotomono-Extralight Fonts
    strncpy(font_config.Name, "Robotomono-Extralight", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoMono_ExtraLight_compressed_data, RobotoMono_ExtraLight_compressed_size, 23.0f, &font_config);

    // Robotomono-Extralightitalic Fonts
    strncpy(font_config.Name, "Robotomono-Extralightitalic", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoMono_ExtraLightItalic_compressed_data, RobotoMono_ExtraLightItalic_compressed_size, 23.0f, &font_config);

    // Robotomono-Italic Fonts
    strncpy(font_config.Name, "Robotomono-Italic", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoMono_Italic_compressed_data, RobotoMono_Italic_compressed_size, 23.0f, &font_config);

    // Robotomono-Light Fonts
    strncpy(font_config.Name, "Robotomono-Light", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoMono_Light_compressed_data, RobotoMono_Light_compressed_size, 23.0f, &font_config);

    // Robotomono-Lightitalic Fonts
    strncpy(font_config.Name, "Robotomono-Lightitalic", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoMono_LightItalic_compressed_data, RobotoMono_LightItalic_compressed_size, 23.0f, &font_config);

    // Robotomono-Medium Fonts
    strncpy(font_config.Name, "Robotomono-Medium", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoMono_Medium_compressed_data, RobotoMono_Medium_compressed_size, 23.0f, &font_config);

    // Robotomono-Mediumitalic Fonts
    strncpy(font_config.Name, "Robotomono-Mediumitalic", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoMono_MediumItalic_compressed_data, RobotoMono_MediumItalic_compressed_size, 23.0f, &font_config);

    // Robotomono-Regular Fonts
    strncpy(font_config.Name, "Robotomono-Regular", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoMono_Regular_compressed_data, RobotoMono_Regular_compressed_size, 23.0f, &font_config);

    // Robotomono-Semibold Fonts
    strncpy(font_config.Name, "Robotomono-Semibold", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoMono_SemiBold_compressed_data, RobotoMono_SemiBold_compressed_size, 23.0f, &font_config);

    // Robotomono-Semibolditalic Fonts
    strncpy(font_config.Name, "Robotomono-Semibolditalic", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoMono_SemiBoldItalic_compressed_data, RobotoMono_SemiBoldItalic_compressed_size, 23.0f, &font_config);

    // Robotomono-Thin Fonts
    strncpy(font_config.Name, "Robotomono-Thin", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoMono_Thin_compressed_data, RobotoMono_Thin_compressed_size, 23.0f, &font_config);

    // Robotomono-Thinitalic Fonts
    strncpy(font_config.Name, "Robotomono-Thinitalic", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoMono_ThinItalic_compressed_data, RobotoMono_ThinItalic_compressed_size, 23.0f, &font_config);

    // Robotoslab-Black Fonts
    strncpy(font_config.Name, "Robotoslab-Black", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoSlab_Black_compressed_data, RobotoSlab_Black_compressed_size, 23.0f, &font_config);

    // Robotoslab-Bold Fonts
    strncpy(font_config.Name, "Robotoslab-Bold", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoSlab_Bold_compressed_data, RobotoSlab_Bold_compressed_size, 23.0f, &font_config);

    // Robotoslab-Extrabold Fonts
    strncpy(font_config.Name, "Robotoslab-Extrabold", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoSlab_ExtraBold_compressed_data, RobotoSlab_ExtraBold_compressed_size, 23.0f, &font_config);

    // Robotoslab-Extralight Fonts
    strncpy(font_config.Name, "Robotoslab-Extralight", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoSlab_ExtraLight_compressed_data, RobotoSlab_ExtraLight_compressed_size, 23.0f, &font_config);

    // Robotoslab-Light Fonts
    strncpy(font_config.Name, "Robotoslab-Light", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoSlab_Light_compressed_data, RobotoSlab_Light_compressed_size, 23.0f, &font_config);

    // Robotoslab-Medium Fonts
    strncpy(font_config.Name, "Robotoslab-Medium", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoSlab_Medium_compressed_data, RobotoSlab_Medium_compressed_size, 23.0f, &font_config);

    // Robotoslab-Regular Fonts
    strncpy(font_config.Name, "Robotoslab-Regular", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoSlab_Regular_compressed_data, RobotoSlab_Regular_compressed_size, 23.0f, &font_config);

    // Robotoslab-Semibold Fonts
    strncpy(font_config.Name, "Robotoslab-Semibold", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoSlab_SemiBold_compressed_data, RobotoSlab_SemiBold_compressed_size, 23.0f, &font_config);

    // Robotoslab-Thin Fonts
    strncpy(font_config.Name, "Robotoslab-Thin", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(RobotoSlab_Thin_compressed_data, RobotoSlab_Thin_compressed_size, 23.0f, &font_config);

    // Finalizing font load
    ImGui::GetIO().Fonts->Build();
}
