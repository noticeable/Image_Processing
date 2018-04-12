#!/bin/bash

cp -fr data/* bin/
sync
cd bin

folder=$PWD

rm -fr Output

mkdir Output

./denoising Lena_mixed.raw 512 512 3 5 Lena.raw
cp -fr Lena.raw Lena.rgb
convert -size 512x512 -depth 8 Lena.rgb Output/Lena.jpg
mv -f output_Denoising.raw output_Denoising.rgb
convert -size 512x512 -depth 8 output_Denoising.rgb Output/output_Denoising.jpg


./histogramEqualization Desk.raw 400 300 3 
cp -fr Desk.raw Desk.rgb
convert -size 400x300 -depth 8 Desk.rgb Output/Desk.jpg
mv -f outputhist.raw outputhist.rgb
convert -size 400x300 -depth 8 outputhist.rgb Output/outputhist.jpg


./imageResize Airplane.raw 512 512 650 650 3
cp -fr Airplane.raw Airplane.rgb
convert -size 512x512 -depth 8 Airplane.rgb Output/Airplane.jpg
mv -f resizedImage.raw resizedImage.rgb
convert -size 650x650 -depth 8 resizedImage.rgb Output/resizedImage.jpg

./oilPaintingEffect Barn.raw 380 275 3 5 64
cp -fr Barn.raw Barn.rgb
convert -size 380x275 -depth 8 Barn.rgb Output/Barn.jpg
mv -f oilPainting.raw oilPainting.rgb
convert -size 380x275 -depth 8 oilPainting.rgb Output/oilPainting.jpg



./rgb2gray Tiffany.raw 512 512 3
cp -fr Tiffany.raw Tiffany.rgb
convert -size 512x512 -depth 8 Tiffany.rgb Output/Tiffany.jpg
mv -f output_luminosityMethod.raw output_luminosityMethod.gray
convert -size 512x512 -depth 8 output_luminosityMethod.gray Output/output_luminosityMethod.jpg

mv -f output_LightnessMethod.raw output_LightnessMethod.gray
convert -size 512x512 -depth 8 output_LightnessMethod.gray Output/output_LightnessMethod.jpg

mv -f output_AverageMethod.raw output_AverageMethod.gray
convert -size 512x512 -depth 8 output_AverageMethod.gray Output/output_AverageMethod.jpg


cp -fr colorchecker.raw colorchecker.gray
convert -size 512x512 -depth 8 colorchecker.gray Output/colorchecker.jpg
./digitalHalftoning colorchecker.raw 512 512 1 1
mv -f DigitalHalftoning_fixedThres.raw DigitalHalftoning_fixedThres.gray
convert -size 512x512 -depth 8 DigitalHalftoning_fixedThres.gray Output/DigitalHalftoning_fixedThres.jpg
./digitalHalftoning colorchecker.raw 512 512 1 2
mv -f DigitalHalftoning_randomThres.raw DigitalHalftoning_randomThres.gray
convert -size 512x512 -depth 8 DigitalHalftoning_randomThres.gray Output/DigitalHalftoning_randomThres.jpg
./digitalHalftoning colorchecker.raw 512 512 1 3
mv -f bayers_dithering_2x2.raw bayers_dithering_2x2.gray
convert -size 512x512 -depth 8 bayers_dithering_2x2.gray Output/bayers_dithering_2x2.jpg
mv -f bayers_dithering_4x4.raw bayers_dithering_4x4.gray
convert -size 512x512 -depth 8 bayers_dithering_4x4.gray Output/bayers_dithering_4x4.jpg
mv -f bayers_dithering_8x8.raw bayers_dithering_8x8.gray
convert -size 512x512 -depth 8 bayers_dithering_8x8.gray Output/bayers_dithering_8x8.jpg


./geometricWarping panda.raw 512 512 3
cp -fr panda.raw panda.rgb
convert -size 512x512 -depth 8 panda.rgb Output/panda.jpg
mv -f warpedOutput.raw warpedOutput.rgb
convert -size 512x512 -depth 8 warpedOutput.rgb Output/warpedOutput.jpg
mv -f dewarpedOutput.raw dewarpedOutput.rgb
convert -size 512x512 -depth 8 dewarpedOutput.rgb Output/dewarpedOutput.jpg



./panoramicStitching middle.raw 480 640 3 left.raw right.raw
cp -fr left.raw left.rgb
convert -size 480x640 -depth 8 left.rgb Output/left.jpg
cp -fr middle.raw middle.rgb
convert -size 480x640 -depth 8 middle.rgb Output/middle.jpg
cp -fr right.raw right.rgb
convert -size 480x640 -depth 8 right.rgb Output/right.jpg
mv -f homography.raw homography.rgb
convert -size 5000x5000 -depth 8 homography.rgb Output/homography.jpg


cp -fr stars.raw stars.gray
convert -size 640x480 -depth 8 stars.gray Output/stars.jpg
./morphology stars.raw 640 480 1 1 1
mv -f Output_shrinked.raw Output_shrinked.gray
convert -size 640x480 -depth 8 Output_shrinked.gray Output/Output_shrinked.jpg
./morphology jigsaw_1.raw 100 100 1 2 0
mv -f Output_Thinned.raw Output_Thinned.gray
convert -size 100x100 -depth 8 Output_Thinned.gray Output/Output_Thinned.jpg
./morphology jigsaw_2.raw 100 100 1 3 0
mv -f Output_Skeletonized.raw Output_Skeletonized.gray
convert -size 100x100 -depth 8 Output_Skeletonized.gray Output/Output_Skeletonized.jpg

./textureClassification 
cp -fr texture10.raw texture10.gray
convert -size 128x128 -depth 8 texture10.gray Output/texture10.jpg
cp -fr texture11.raw texture11.gray
convert -size 128x128 -depth 8 texture11.gray Output/texture11.jpg
cp -fr texture12.raw texture12.gray
convert -size 128x128 -depth 8 texture12.gray Output/texture12.jpg
cp -fr texture14.raw texture14.gray
convert -size 128x128 -depth 8 texture14.gray Output/texture14.jpg
cp -fr texture1.raw texture1.gray
convert -size 128x128 -depth 8 texture1.gray Output/texture1.jpg
cp -fr texture2.raw texture2.gray
convert -size 128x128 -depth 8 texture2.gray Output/texture2.jpg
cp -fr texture3.raw texture3.gray
convert -size 128x128 -depth 8 texture3.gray Output/texture3.jpg
cp -fr texture4.raw texture4.gray
convert -size 128x128 -depth 8 texture4.gray Output/texture4.jpg
cp -fr texture5.raw texture5.gray
convert -size 128x128 -depth 8 texture5.gray Output/texture5.jpg
cp -fr texture6.raw texture6.gray
convert -size 128x128 -depth 8 texture6.gray Output/texture6.jpg
cp -fr texture7.raw texture7.gray
convert -size 128x128 -depth 8 texture7.gray Output/texture7.jpg
cp -fr texture8.raw texture8.gray
convert -size 128x128 -depth 8 texture8.gray Output/texture8.jpg
cp -fr texture9.raw texture9.gray
convert -size 128x128 -depth 8 texture9.gray Output/texture9.jpg

./zeroCrossingEdgeDetector Boat.raw 321 481 3
cp -fr Boat.raw Boat.rgb
convert -size 321x481 -depth 8 Boat.rgb Output/Boat.jpg
mv -f zerocrossing_norm.raw zerocrossing_norm.gray
convert -size 321x481 -depth 8 zerocrossing_norm.gray Output/zerocrossing_norm.jpg
mv -f zerocrossing.raw zerocrossing.gray
convert -size 321x481 -depth 8 zerocrossing.gray Output/zerocrossing.jpg

./SobelEdgeDetector Boat.raw 321 481 3
cp -fr Boat.raw Boat.rgb
convert -size 321x481 -depth 8 Boat.rgb Output/Boat.jpg
mv -f SobelDetector_noisyImage_norm.raw SobelDetector_noisyImage_norm.gray
convert -size 321x481 -depth 8 SobelDetector_noisyImage_norm.gray Output/SobelDetector_noisyImage_norm.jpg
mv -f SobelDetector_noisy_edges.raw SobelDetector_noisy_edges.gray
convert -size 321x481 -depth 8 SobelDetector_noisy_edges.gray Output/SobelDetector_noisy_edges.jpg



./textureSegmentation comb.raw 600 450 1 
cp -fr comb.raw comb.gray
convert -size 600x450 -depth 8 comb.gray Output/comb.jpg
mv -f segmented_texture.raw segmented_texture.gray
convert -size 600x450 -depth 8 segmented_texture.gray Output/segmented_texture.jpg






# rm -fr *.raw *.rgb












