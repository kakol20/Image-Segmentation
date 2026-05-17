# Image Segmentation
Image Segmentation using a mixture of Mean Shift Clustering and K-Means Clustering  
Using Gaussian kernel

## JSON

### Settings
```json
{
	"bandwidth": 0.05,
	"epsilon": 0.01,
	"max": -1
}
```

#### `bandwidth`
- Radius of influence
- Uses OkLab

#### `epsilon`
- The convergence value for early exit
- Uses OkLab

#### `max`
- The maximum amount of colours generated
- If `<=0`
	- Will first merge generated colours from mean shift clustering using `epsilon` value
	- Then will output the result - skipping k-means clustering

# Credits
[stb_image](https://github.com/nothings/stb) Sean Barrett  
[OkLab](https://bottosson.github.io/posts/oklab/) by Björn Ottosson  
[json](https://github.com/nlohmann/json) by Niels Lohmann