<h1 align="center">bangumi-status-card</h1>

---

## Feature

Display bangumi status in a form of a svg card, which can be attached to any kind of your personal introduction, such as blog, github profile and so on.

Unfortunately, since github svg card disallow remote image loading, there is currently no way to involve covers of anime in the github related cards. QAQ. But for normal blogs the covers can be done right.

## How to use

For markdown:

```markdown
![](bangumi-status-card.dvdbr3o.top/details?username=dvdbr3o)
```

For html:

```html
<img src="bangumi-status-card.dvdbr3o.top/details?username=dvdbr3o"></img>
```

### Parameters

| Parameter       | Type    | Default | Description                                                                                  |
| --------------- | ------- | ------- | -------------------------------------------------------------------------------------------- |
| username        | string  |         | The name of user.                                                                            |
| top             | number? | 0       | The number of anime listed.<br />0 for infinity                                              |
| collection_type | enum?   | 2       | 1: Want to watch<br /> 2: Watched <br /> 3: On watching <br /> 4: Delayed <br /> 5: Deserted |
| title           | bool?   | true    | Requires title above.                                                                        |
| chinese         | bool?   | false   | Whether to use Chinese for bangumi name.                                                     |

These parameters are for interface `details`. Later we will support another type `overview`. And a specific github parameter to disable avatar which requires remote image loading.

## Deploy

## Related Projects

- [anuraghazra/github-readme-stats](https://github.com/anuraghazra/github-readme-stats): Status card about github user info.
- [open-ani/animeko](https://github.com/open-ani/animeko): A cross-platform and modern bangumi app that is highly recommended, anime status actions on which will be synchroized to bangumi, compatible with this repo.
- [bangumi/api](https://github.com/bangumi/api): The api of bangumi.
